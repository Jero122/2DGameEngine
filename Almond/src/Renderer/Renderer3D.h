#pragma once
#include "Renderer/Renderer.h"

class Renderer3D : public Renderer
{
private:
	struct ModelTuple
	{
		std::shared_ptr<Model> model;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		//editor only
		int entityID;
	};

	struct PointLight
	{
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float constant;
		float linear;
		float quadratic;
	};

	struct SpotLight
	{
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 direction;
		float innerCutOff;
		float outerCutoff;
	};

	struct DirectionalLight
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 direction;
		bool enabled = false;
	};

	std::vector<ModelTuple> m_Models;
	std::vector<PointLight> m_pointLights;
	DirectionalLight m_directional_light = {{0,0,0} , {0,0,0} ,{0,0,0} , {0,0,0} , false};

	std::unique_ptr<Shader> m_SkyboxShader;
	std::unique_ptr<GLVertexArray> m_SkyboxVAO;
	std::unique_ptr<GLVertexBuffer> m_SkyboxVBO;
	unsigned int SkyboxTexture;



public:
	Renderer3D();
	~Renderer3D() override;

	void Shutdown() override;
	void BeginScene(EditorCamera& camera) override;
	void EndScene() override;
	void ResetStats() override;
	RenderStats GetStats() override;

	void Submit(std::shared_ptr<Model> model, const glm::vec3 position, const glm::vec3 rotation,
		const glm::vec3 scale);
	void Submit(std::shared_ptr<Model> model, const glm::vec3 position, const glm::vec3 rotation,
		const glm::vec3 scale, int entityID);

	void SubmitPointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
	void SubmitSpotLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction, float innerCutoff, float outterCutoff);
	void SetDirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);
};
