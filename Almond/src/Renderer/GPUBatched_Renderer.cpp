#include <array>

#include "GPUBatched_Renderer.h"
#include "OpenGLRenderCommand.h"

GPUBatched_Renderer::GPUBatched_Renderer()
{
	std::string shaderPath("resources/shaders/BasicShader_GPUMatrices.glsl");
	m_Shader = std::make_unique<Shader>();
	m_Shader->init(shaderPath);
	m_Shader->use();

	m_QuadBuffer = new Quad[MAX_BATCH_COUNT];

	m_VertexArray = std::make_unique<OpenGLVertexArray>();
	m_VertexArray->Bind();

	m_VertexBuffer = std::make_unique<OpenGLVertexBuffer>(nullptr, MAX_VERTEX_COUNT * sizeof(Quad::Vertex));

	BufferLayout layout;
	layout.AddAttribute({ "aPos", BufferAttribType::Float3, false });
	layout.AddAttribute({ "aTranslate", BufferAttribType::Float3, false });
	layout.AddAttribute({ "aScale", BufferAttribType::Float2, false });
	layout.AddAttribute({ "aRotate", BufferAttribType::Float, false });
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

	m_QuadBufferPtr = m_QuadBuffer;

	m_IndexBuffer = std::make_unique<OpenGLIndexBuffer>(indices, sizeof(indices));


	int32_t samplers[s_MaxTextureSlots];
	for (uint32_t i = 0; i < s_MaxTextureSlots; ++i)
	{
		samplers[i] = i;
	}

	m_Vertices[0] = { 0.5f, 0.5f, 0.0f, 1.0f }; // top right
	m_Vertices[1] = { 0.5f, -0.5f, 0.0f, 1.0f };  // bottom right
	m_Vertices[2] = { -0.5f, -0.5f, 0.0f, 1.0f };  // bottom left
	m_Vertices[3] = { -0.5f,  0.5f, 0.0f, 1.0f };   // top left 


	m_Shader->setIntArray("uTextures", samplers, s_MaxTextureSlots);
}


void GPUBatched_Renderer::Shutdown()
{
	/*delete[] quadBuffer;*/
}


void GPUBatched_Renderer::BeginScene(EditorCamera& camera)
{
	projection = camera.GetProjectionMatrix();
	view = camera.GetViewMatrix();

	OpenGLRenderCommand::ClearColor(62.0f / 255.0f, 62.0f / 255.0f, 58.0f / 255.0f, 1.0f);
	OpenGLRenderCommand::Clear();
	BeginBatch();
}


void GPUBatched_Renderer::EndScene()
{
	EndBatch();
	Flush();
	OpenGLRenderCommand::UnbindFrameBuffer();
}

void GPUBatched_Renderer::Submit(const glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, int textureID, glm::vec2* texCoords)
{
	m_RenderStats.QuadCount++;

	if (indexCount >= MAX_INDEX_COUNT)
	{
		NextBatch();
	}

	float textureIndex = 0;

	//If textureID is already in batch
	for (unsigned int i = 0; i < m_TextureSlotIndex; ++i)
	{
		if (m_TextureSlots[i] == textureID)
		{
			//Set the textureIndex for this quad to the one already in the batch;
			textureIndex = i;
			break;
		}
	}

	//If textureID was not in batch
	if (textureIndex == 0 && textureID > 0)
	{
		//If all the textureSlots have been occupied, end batch
		if (m_TextureSlotIndex >= s_MaxTextureSlots)
		{
			NextBatch();
		}

		textureIndex = m_TextureSlotIndex;
		m_TextureSlots[m_TextureSlotIndex] = textureID;
		m_TextureSlotIndex++;
	}

	//encoding rgba colour into a single unsigned byte
	int r = color.r * 255;
	int g = color.g * 255;
	int b = color.b * 255;
	int a = color.a * 255;

	unsigned int c = a << 24 | b << 16 | g << 8 | r;
	float rot = -glm::radians(rotation);
	
	//top right
	m_QuadBufferPtr->topRight = Quad::Vertex{m_Vertices[0], position, scale,rot, c, texCoords[0], textureIndex};
	//bottom right
	m_QuadBufferPtr->bottomRight = Quad::Vertex{ m_Vertices[1], position, scale,rot, c, texCoords[1], textureIndex };
	//bottom left
	m_QuadBufferPtr->bottomLeft = Quad::Vertex{ m_Vertices[2], position, scale,rot, c, texCoords[2], textureIndex };;
	//top left
	m_QuadBufferPtr->topLeft = Quad::Vertex{ m_Vertices[3], position, scale,rot, c, texCoords[3], textureIndex };

	m_QuadBufferPtr++;

	indexCount += 6;

}

void GPUBatched_Renderer::BeginBatch()
{
	m_QuadBufferPtr = m_QuadBuffer;
	indexCount = 0;
	m_TextureSlotIndex = 1; //Set to 1 because 0 is reserved for colored quads
}

void GPUBatched_Renderer::EndBatch()
{
	m_RenderStats.DrawCalls++;
	uint32_t size = (m_QuadBufferPtr - m_QuadBuffer) * sizeof(Quad);
	m_VertexBuffer->Bind();
	m_VertexBuffer->RedefineDataStore(0, size, m_QuadBuffer);
}


void GPUBatched_Renderer::Flush()
{
	m_Shader->use();
	m_Shader->setMat4("uView", view);
	m_Shader->setMat4("uProjection", projection);

	for (unsigned int i = 1; i < m_TextureSlotIndex; ++i)
	{
		OpenGLRenderCommand::BindTexture(m_TextureSlots[i], i);
	}

	m_VertexArray->Bind();
	m_IndexBuffer->Bind();

	OpenGLRenderCommand::DrawElementsTriangle(indexCount, 0);
}


void GPUBatched_Renderer::NextBatch()
{
	EndBatch();
	Flush();
	BeginBatch();
}

void GPUBatched_Renderer::ResetStats()
{
	memset(&m_RenderStats, 0, sizeof(RenderStats));
}

GPUBatched_Renderer::RenderStats GPUBatched_Renderer::GetStats()
{
	return m_RenderStats;
}
