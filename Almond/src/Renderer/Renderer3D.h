#pragma once
#include "GLFramework/GLCubeMap.h"
#include "GLFramework/GLMesh.h"
#include "Mesh/AMesh.h"
#include "Mesh/ProceduralMesh.h"
#include "Renderer/Renderer.h"


class Renderer3D : public Renderer
{
private:
	struct RenderObject
	{
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		//editor only
		int entityID;
	};

	struct ModelTuple: RenderObject
	{
		std::shared_ptr<Model> model;
	};

	struct StaticMeshTuple: RenderObject
	{
		std::shared_ptr<GLMesh> mesh;
	};

	struct ProceduralMeshTuple: RenderObject
	{
		std::shared_ptr<ProceduralMesh> proceduralMesh;
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
	std::vector<StaticMeshTuple> m_Meshes;
	std::vector<ProceduralMeshTuple> m_ProceduralMeshes;
	std::shared_ptr<Texture> m_brdf;

	std::vector<PointLight> m_pointLights;
	DirectionalLight m_directional_light = {{0,0,0} , {0,0,0} ,{0,0,0} , {0,0,0} , false};

	std::unique_ptr<Shader> m_SkyboxShader;
	std::shared_ptr<GLCubeMap> m_SkyboxTexture;
	std::unique_ptr<GLVertexArray> m_SkyboxVAO;

public:
	Renderer3D();
	~Renderer3D() override;

	void Shutdown() override;
	void BeginScene(EditorCamera& camera) override;
	void EndScene() override;
	void ResetStats() override;
	RenderStats GetStats() override;

	//Old Models
	void Submit(std::shared_ptr<Model> model, const glm::vec3 position, const glm::vec3 rotation,
		const glm::vec3 scale);
	void Submit(std::shared_ptr<Model> model, const glm::vec3 position, const glm::vec3 rotation,
		const glm::vec3 scale, int entityID);

	//New Mesh Format
	void Submit(std::shared_ptr<GLMesh> mesh, const glm::vec3 position, const glm::vec3 rotation,
		const glm::vec3 scale);
	void Submit(std::shared_ptr<GLMesh> mesh, const glm::vec3 position, const glm::vec3 rotation,
		const glm::vec3 scale, int entityID);

	//Procedural Mesh
	void Submit(std::shared_ptr<ProceduralMesh> model, const glm::vec3 position, const glm::vec3 rotation,
		const glm::vec3 scale);
	void Submit(std::shared_ptr<ProceduralMesh> model, const glm::vec3 position, const glm::vec3 rotation,
		const glm::vec3 scale, int entityID);

	void SubmitPointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
	void SubmitSpotLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction, float innerCutoff, float outterCutoff);
	void SetDirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);
};
