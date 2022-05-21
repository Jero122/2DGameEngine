#include "Renderer/Renderer3D.h"
#include "OpenGLRenderCommand.h"

Renderer3D::Renderer3D()
{

	stbi_set_flip_vertically_on_load(true);
	std::string shaderPath("resources/shaders/Lit3D.glsl");
	m_Shader = std::make_unique<Shader>();
	m_Shader->init(shaderPath);
	m_Shader->use();

	backpack = Model("Resources/Models/backpack/backpack.obj");
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
	m_Shader->setMat4("uView", m_ViewMatrix);
	m_Shader->setMat4("uProjection", m_ProjectionMatrix);
	m_Shader->setVec3("uViewPos", m_ViewPosition);
	// directional light
	m_Shader->setVec3("uDirectionalLight.direction", -0.2f, -1.0f, -0.3f);
	m_Shader->setVec3("uDirectionalLight.ambient", 0.05f, 0.05f, 0.05f);
	m_Shader->setVec3("uDirectionalLight.diffuse", 0.4f, 0.4f, 0.4f);
	m_Shader->setVec3("uDirectionalLight.specular", 0.5f, 0.5f, 0.5f);

	m_Shader->setInt("uMaterial.diffuse", 0);
	m_Shader->setVec3("uMaterial.specular", 1.0f, 1.0f, 1.0f);
	m_Shader->setFloat("uMaterial.shininess", 512.0f);

	m_Shader->setVec3("uLight.direction", -0.2f, -1.0f, -0.3f);
	m_Shader->setVec3("uLight.ambient", 0.2f, 0.2f, 0.2f);
	m_Shader->setVec3("uLight.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
	m_Shader->setFloat("uLight.constant", 1.0f);
	m_Shader->setFloat("uLight.linear", 0.09f);
	m_Shader->setFloat("uLight.quadratic", 0.032f);

	/*for (auto model : m_Models)
	{


		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down

		m_Shader->setMat4("uModel", modelMatrix);
		model->Draw(*m_Shader);
	}*/

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
	m_Shader->setMat4("uModel", model);
	backpack.Draw(*m_Shader);
	//m_Models.clear();
}

void Renderer3D::Submit(const glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, int textureID,
	glm::vec2* texCoords)
{
}

void Renderer3D::Submit(std::shared_ptr<Model> model)
{
	m_Models.push_back(model);
}

void Renderer3D::ResetStats()
{
	memset(&m_RenderStats, 0, sizeof(RenderStats));
}

Renderer3D::RenderStats Renderer3D::GetStats()
{
	return m_RenderStats;
}
