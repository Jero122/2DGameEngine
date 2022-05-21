#include <array>

#include "Renderer2D.h"
#include <future>
#include "OpenGLRenderCommand.h"

Renderer2D::Renderer2D()
{
	/*m_Batches = std::vector<Batch>();*/
	m_CurrentBatch = new Batch();

	std::string shaderPath("resources/shaders/BasicShader_CPUMatrices.glsl");
	m_Shader = std::make_unique<Shader>();
	m_Shader->init(shaderPath);
	m_Shader->use();

	m_VertexArray = std::make_unique<OpenGLVertexArray>();
	m_VertexArray->Bind();
	m_VertexBuffer = std::make_unique<OpenGLVertexBuffer>(nullptr, MAX_VERTEX_COUNT * sizeof(Quad::Vertex));

	BufferLayout layout;
	layout.AddAttribute({ "aPos", BufferAttribType::Float3, false });
	layout.AddAttribute({ "aColor", BufferAttribType::UnsignedByte, true });
	layout.AddAttribute({ "aTexCoords", BufferAttribType::Float2, false });
	layout.AddAttribute({ "aTexId", BufferAttribType::Float, false });
	m_VertexBuffer->SetLayout(layout);

	//EBO
	//Populates INDICES array as that will remain the same
	uint32_t indices[MAX_INDEX_COUNT];
	uint32_t offset = 0;
	for (int i = 0; i < MAX_INDEX_COUNT; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 3 + offset;

		indices[i + 3] = 1 + offset;
		indices[i + 4] = 2 + offset;
		indices[i + 5] = 3 + offset;

		offset += 4;
	}

	m_IndexBuffer = std::make_unique<OpenGLIndexBuffer>(indices, sizeof(indices));

	int32_t samplers[MAX_TEXTURE_SLOTS];
	for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; ++i)
	{
		samplers[i] = i;
	}

	m_Vertices[0] = { 0.5f, 0.5f, 0.0f, 1.0f }; // top right
	m_Vertices[1] = { 0.5f, -0.5f, 0.0f, 1.0f };  // bottom right
	m_Vertices[2] = { -0.5f, -0.5f, 0.0f, 1.0f };  // bottom left
	m_Vertices[3] = { -0.5f,  0.5f, 0.0f, 1.0f };   // top left 

	m_Shader->setIntArray("uTextures", samplers, MAX_TEXTURE_SLOTS);
}

void Renderer2D::Shutdown()
{

}

void Renderer2D::BeginScene(EditorCamera& camera)
{
	m_ProjectionMatrix = camera.GetProjectionMatrix();
	m_ViewMatrix = camera.GetViewMatrix();

	OpenGLRenderCommand::ClearColor(62.0f / 255.0f, 62.0f / 255.0f, 58.0f / 255.0f, 1.0f);
	OpenGLRenderCommand::Clear();
}

void Renderer2D::EndScene()
{
	//Put current batch in list
	m_Batches->push_back(m_CurrentBatch);

	/*//APPROACH 1: PROCESS ALL BATCHES IN MAIN THREAD
	ProcessBatches(m_BatchBufferData, m_Batches);*/

	/*//APPROACH 2: CREATE ASYNC THREAD WITH ALL BATCHES
	auto future = std::async(std::launch::async, ProcessBatches, m_BatchBufferData, m_Batches);
	future.get();*/

	//APPROACH 3: CREATE ASYNC THREAD FOR EACH BATCH AND WAIT UNTIL THEY ARE ALL DONE
	for (auto batch : *m_Batches)
	{
		m_Futures.push_back(std::async(std::launch::async, ProcessBatch, m_BatchBufferData, batch));
	}
	for_each(m_Futures.begin(), m_Futures.end(), [](std::future<void>& x) {x.get() ; });


	//As the BufferData list gets populated, render the batches
	for (auto bufferData : *m_BatchBufferData)
	{
		uint32_t size = (bufferData->QuadBufferPtr - bufferData->QuadBuffer) * sizeof(Quad);
		
		m_VertexBuffer->RedefineDataStore(0, size, bufferData->QuadBuffer);
		m_Shader->use();
		m_Shader->setMat4("uView", m_ViewMatrix);
		m_Shader->setMat4("uProjection", m_ProjectionMatrix);

		m_VertexBuffer->Bind();
		m_VertexArray->Bind();
		m_IndexBuffer->Bind();
		for (unsigned int i = 1; i < bufferData->CurrentTextureSlotIndex; ++i)
		{
			OpenGLRenderCommand::BindTexture(bufferData->TextureSlots[i], i);
		}
		OpenGLRenderCommand::DrawElementsTriangle(bufferData->IndexCount, 0);
	}

	OpenGLRenderCommand::UnbindFrameBuffer();


	//clear all batches
	for (auto batch : *m_Batches)
	{
		//free memory
		delete batch;
	}
	m_Batches->clear();
	//delete all bufferData
	for (auto bufferData : *m_BatchBufferData)
	{
		//free memory
		delete bufferData;
	}
	m_BatchBufferData->clear();
	//clear futures
	m_Futures.clear();
	m_CurrentBatch = new Batch();
}

void Renderer2D::Submit(const glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, int textureID, glm::vec2* texCoords)
{
	float textureIndex = 0;
	//If textureID is already in batch
	for (unsigned int i = 0; i < m_CurrentBatch->TextureSlotIndex; ++i)
	{
		if (m_CurrentBatch->TextureSlots[i] == textureID)
		{
			//Set the textureIndex for this quad to the one already in the batch;
			textureIndex = i;
			break;
		}
	}

	//If textureID was not in batch
	if (textureIndex == 0 && textureID > 0)
	{
		textureIndex = m_CurrentBatch->TextureSlotIndex;
		m_CurrentBatch->TextureSlots[m_CurrentBatch->TextureSlotIndex] = textureID;
		m_CurrentBatch->TextureSlotIndex++;
	}

	m_CurrentBatch->AddQuad(position,rotation,scale,color,textureID,texCoords);
	if (m_CurrentBatch->QuadCount >= MAX_BATCH_COUNT || m_CurrentBatch->TextureSlotIndex >= MAX_TEXTURE_SLOTS)
	{
		m_Batches->push_back(m_CurrentBatch);
		//NEED TO DELETE ALL BATCHES
		m_CurrentBatch = new Batch();
	
	}


}

void Renderer2D::Submit(std::shared_ptr<Model> model)
{
}

void Renderer2D::ProcessBatch(std::shared_ptr<std::vector<BatchBuffer*>> buffers, Batch* batch)
{
	BatchBuffer* buffer = new BatchBuffer();
	for (auto quad : batch->Quads)
	{
		glm::mat4 transformMatrix{ 1.0f };
		if (quad.Rotation == 0)
		{
			transformMatrix = glm::translate(glm::mat4(1.0f), quad.Position)
				* glm::scale(glm::mat4(1.0f), { quad.Scale.x, quad.Scale.y, 1.0f });
		}
		else
		{
			transformMatrix = glm::translate(glm::mat4(1.0f), quad.Position)
				* glm::rotate(glm::mat4(1.0f), glm::radians(quad.Rotation), { 0.0f,0.0f,1.0f })
				* glm::scale(glm::mat4(1.0f), { quad.Scale.x, quad.Scale.y, 1.0f });
		}


		float textureIndex = 0;
		//If textureID is already in batch
		for (unsigned int i = 0; i < batch->TextureSlotIndex; ++i)
		{
			if (buffer->TextureSlots[i] == quad.TextureID)
			{
				//Set the textureIndex for this quad to the one already in the batch;
				textureIndex = i;
				break;
			}
		}

		//If textureID was not in batch
		if (textureIndex == 0 && quad.TextureID > 0)
		{
			textureIndex = buffer->CurrentTextureSlotIndex;
			buffer->TextureSlots[buffer->CurrentTextureSlotIndex] = quad.TextureID;
			buffer->CurrentTextureSlotIndex++;
		}

		//encoding rgba colour into a single unsigned byte
		int r = quad.Color.r * 255;
		int g = quad.Color.g * 255;
		int b = quad.Color.b * 255;
		int a = quad.Color.a * 255;

		unsigned int c = a << 24 | b << 16 | g << 8 | r;

		buffer->QuadBufferPtr->topRight = Quad::Vertex{ transformMatrix * m_Vertices[0], c, quad.TexCoords[0], textureIndex };
		//bottom right
		buffer->QuadBufferPtr->bottomRight = Quad::Vertex{ transformMatrix * m_Vertices[1],c, quad.TexCoords[1], textureIndex };
		//bottom left
		buffer->QuadBufferPtr->bottomLeft = Quad::Vertex{ transformMatrix * m_Vertices[2], c, quad.TexCoords[2], textureIndex };
		//top left
		buffer->QuadBufferPtr->topLeft = Quad::Vertex{ transformMatrix * m_Vertices[3],c, quad.TexCoords[3], textureIndex };
		buffer->QuadBufferPtr++;
		buffer->IndexCount += 6;
	}

	//SHARED RESOURCE//
	buffer->TextureSlots = batch->TextureSlots;

	std::lock_guard <std::mutex> lock(mutex);
	buffers->push_back(buffer);
}


void Renderer2D::ResetStats()
{
	memset(&m_RenderStats, 0, sizeof(RenderStats));
}

Renderer2D::RenderStats Renderer2D::GetStats()
{
	return m_RenderStats;
}
