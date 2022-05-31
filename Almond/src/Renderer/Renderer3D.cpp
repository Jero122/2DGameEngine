#include "Renderer/Renderer3D.h"
#include "OpenGLRenderCommand.h"
#include "stb/stb_image.h"

Renderer3D::Renderer3D()
{

	stbi_set_flip_vertically_on_load(true);
	std::string shaderPath("resources/shaders/Lit3D.glsl");
	m_Shader = std::make_unique<Shader>();
	m_Shader->init(shaderPath);
	m_Shader->use();
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
	m_ViewPosition = camera.Position();

	OpenGLRenderCommand::ClearColor(62.0f / 255.0f, 62.0f / 255.0f, 58.0f / 255.0f, 1.0f);
	OpenGLRenderCommand::Clear();
}

void Renderer3D::EndScene()
{
	m_Shader->use();
	m_Shader->setMat4("view", m_ViewMatrix);
	m_Shader->setMat4("projection", m_ProjectionMatrix);
	m_Shader->setVec3("viewPos", m_ViewPosition);

	m_Shader->setInt("material.diffuse", 0);
	m_Shader->setVec3("material.specular", 1.0f, 1.0f, 1.0f);
	m_Shader->setFloat("material.shininess", 512.0f);

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
		model_tuple.model->Draw(*m_Shader);
	}

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
	m_pointLights.clear();
}

void Renderer3D::Submit(std::shared_ptr<Model> model, const glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	m_Models.push_back({model,position,rotation,scale});
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
