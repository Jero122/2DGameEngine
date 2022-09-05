#include "Renderer/Renderer3D.h"
#include "stb/stb_image.h"

std::vector<std::string> faces
{
	"assets/textures/Skybox/Cold Sunset/right.png",
	"assets/textures/Skybox/Cold Sunset/left.png",

	"assets/textures/Skybox/Cold Sunset/top.png",
	"assets/textures/Skybox/Cold Sunset/bottom.png",

	"assets/textures/Skybox/Cold Sunset/front.png",
	"assets/textures/Skybox/Cold Sunset/back.png"
};


Renderer3D::Renderer3D()
{
	std::string shaderPath("assets/shaders/Lambert.glsl");
	m_Shader = std::make_unique<Shader>();
	m_Shader->init(shaderPath);
	m_Shader->use();


	m_SkyboxShader = std::make_unique<Shader>();
	m_SkyboxShader->init(std::string("assets/shaders/SkyBox.glsl"));
	m_SkyboxShader->use();
	m_SkyboxTexture = std::make_shared<GLCubeMap>("assets/textures/Skybox/Cold Sunset/Cold-Sunset-Equirect.hdr");
	glBindTextures(0, 1, &m_SkyboxTexture->id);

	m_VertexArray = std::make_unique<GLVertexArray>();
	m_VertexArray->Bind();

	m_VertexBuffer = std::make_unique<GLVertexBuffer>(nullptr, 0);
	BufferLayout layout;
	layout.AddAttribute({ "aPos", BufferAttribType::Float3, false });
	layout.AddAttribute({ "aNormal", BufferAttribType::Float3, false });
	layout.AddAttribute({ "aTexCoord", BufferAttribType::Float2, false });
	m_VertexBuffer->SetLayout(layout);


	m_SkyboxVAO = std::make_unique<GLVertexArray>();
	m_SkyboxVAO->Bind();
}

Renderer3D::~Renderer3D()
{
}

void Renderer3D::Shutdown()
{
}

void Renderer3D::BeginScene(EditorCamera& camera)
{
	m_ProjectionMatrix = camera.GetProjectionMatrix();
	m_ViewMatrix = camera.GetViewMatrix();
	m_ViewPosition = camera.GetPosition();

}

void Renderer3D::EndScene()
{
	m_Shader->use();
	//glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTexture->id);
	m_Shader->setMat4("view", m_ViewMatrix);
	m_Shader->setMat4("projection", m_ProjectionMatrix);
	m_Shader->setVec3("viewPos", m_ViewPosition);

	m_Shader->setInt("skybox", 0);
	m_Shader->setInt("material.albedoMap", 1);
	m_Shader->setInt("material.aoRoughnessMetallicMap", 2);
	m_Shader->setInt("material.normalMap", 3);
	m_Shader->setInt("material.emissiveMap", 4);
	//Directional Light
	if (m_directional_light.enabled)
	{
		m_Shader->setBool("dirLight.enabled", true);
		m_Shader->setVec3("dirLight.direction", m_directional_light.direction);
		m_Shader->setVec3("dirLight.ambient", m_directional_light.ambient);
		m_Shader->setVec3("dirLight.diffuse", m_directional_light.diffuse);
		m_Shader->setVec3("dirLight.specular", m_directional_light.specular);
	}

	//Point Lights
	for (int i = 0; i < m_pointLights.size(); ++i)
	{
		m_Shader->setVec3("pointLights["+ std::to_string(i) +"].position", m_pointLights[i].position);
		m_Shader->setVec3("pointLights[" + std::to_string(i) + "].ambient", m_pointLights[i].ambient);
		m_Shader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", m_pointLights[i].diffuse);
		m_Shader->setVec3("pointLights[" + std::to_string(i) + "].specular", m_pointLights[i].specular);
		m_Shader->setFloat("pointLights[" + std::to_string(i) + "].constant", m_pointLights[i].constant);
		m_Shader->setFloat("pointLights[" + std::to_string(i) + "].linear", m_pointLights[i].linear);
		m_Shader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", m_pointLights[i].quadratic);
	}

	m_Shader->setInt("numPointLights", m_pointLights.size());

	//Draw Models
	for (auto model_tuple : m_Models)
	{
		glm::mat4 modelMatrix{ 1.0f };
		modelMatrix = glm::translate(glm::mat4(1.0f), model_tuple.position);

		if (model_tuple.rotation.x != 0)
		{
			modelMatrix = modelMatrix * glm::rotate(glm::mat4(1.0f), glm::radians(model_tuple.rotation.x), { 1,0,0 });
		}
		if(model_tuple.rotation.y != 0)
		{
			modelMatrix = modelMatrix * glm::rotate(glm::mat4(1.0f), glm::radians(model_tuple.rotation.y), { 0,1,0 });
		}
		if (model_tuple.rotation.z != 0)
		{
			modelMatrix = modelMatrix * glm::rotate(glm::mat4(1.0f), glm::radians(model_tuple.rotation.z), { 0,0,1 });
		}

		modelMatrix = modelMatrix * glm::scale(glm::mat4(1.0f), model_tuple.scale);

		m_Shader->setMat4("model", modelMatrix);
		m_Shader->setInt("entityID", model_tuple.entityID);
	
		model_tuple.model->Draw(*m_Shader);
	}

	//Draw Meshes
	for (auto mesh_tuple : m_Meshes)
	{
		glm::mat4 modelMatrix{ 1.0f };
		modelMatrix = glm::translate(glm::mat4(1.0f), mesh_tuple.position);

		if (mesh_tuple.rotation.x != 0)
		{
			modelMatrix = modelMatrix * glm::rotate(glm::mat4(1.0f), glm::radians(mesh_tuple.rotation.x), { 1,0,0 });
		}
		if (mesh_tuple.rotation.y != 0)
		{
			modelMatrix = modelMatrix * glm::rotate(glm::mat4(1.0f), glm::radians(mesh_tuple.rotation.y), { 0,1,0 });
		}
		if (mesh_tuple.rotation.z != 0)
		{
			modelMatrix = modelMatrix * glm::rotate(glm::mat4(1.0f), glm::radians(mesh_tuple.rotation.z), { 0,0,1 });
		}

		modelMatrix = modelMatrix * glm::scale(glm::mat4(1.0f), mesh_tuple.scale);

		m_Shader->setMat4("model", modelMatrix);
		m_Shader->setInt("entityID", mesh_tuple.entityID);

		mesh_tuple.mesh->Draw();
	}

	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	m_SkyboxShader->use();
	auto view = glm::mat4(glm::mat3(m_ViewMatrix)); // remove translation from the view matrix
	m_SkyboxShader->setInt("skybox", 0);
	m_SkyboxShader->setMat4("view", view);
	m_SkyboxShader->setMat4("projection", m_ProjectionMatrix);
	// skybox cube
	m_SkyboxVAO->Bind();

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default

	m_Shader->use();
	//Reset Point lights
	for (int i = 0; i < m_pointLights.size(); ++i)
	{
		m_Shader->setVec3("pointLights[0].position", {0,0,0});
		m_Shader->setVec3("pointLights[0].ambient", { 0,0,0 });
		m_Shader->setVec3("pointLights[0].diffuse", { 0,0,0 });
		m_Shader->setVec3("pointLights[0].specular", { 0,0,0 });
		m_Shader->setFloat("pointLights[0].constant", 0);
		m_Shader->setFloat("pointLights[0].linear", 0);
		m_Shader->setFloat("pointLights[0].quadratic", 0);
	}


	//Disable Directional Light
	m_Shader->setBool("dirLight.enabled", false);
	m_directional_light.enabled = false;
	m_Shader->setInt("numPointLights", 0);

	m_Models.clear();
	m_Meshes.clear();
	m_pointLights.clear();
}

void Renderer3D::Submit(std::shared_ptr<Model> model, const glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int entityID)
{
	m_Models.push_back({model,position,rotation,scale,entityID});
}

void Renderer3D::Submit(std::shared_ptr<Model> model, const glm::vec3 position, const glm::vec3 rotation,
	const glm::vec3 scale)
{
}

void Renderer3D::Submit(std::shared_ptr<GLMesh> mesh, const glm::vec3 position, const glm::vec3 rotation,
	const glm::vec3 scale, int entityID)
{
	m_Meshes.push_back({ mesh,position,rotation,scale, entityID });
}

void Renderer3D::Submit(std::shared_ptr<GLMesh> mesh, const glm::vec3 position, const glm::vec3 rotation,
	const glm::vec3 scale)
{
}



void Renderer3D::SubmitPointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                                  float constant, float linear, float quadratic)
{
	m_pointLights.push_back({position, ambient,diffuse,specular, constant,linear,quadratic });
}

void Renderer3D::SubmitSpotLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
	glm::vec3 direction, float innerCutoff, float outterCutoff)
{
}

void Renderer3D::SetDirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
	glm::vec3 direction)
{
	m_directional_light.ambient = ambient;
	m_directional_light.diffuse = diffuse;
	m_directional_light.specular = specular;
	m_directional_light.direction = direction;
	m_directional_light.enabled = true;
}

void Renderer3D::ResetStats()
{
	memset(&m_RenderStats, 0, sizeof(RenderStats));
}

Renderer3D::RenderStats Renderer3D::GetStats()
{
	return m_RenderStats;
}
