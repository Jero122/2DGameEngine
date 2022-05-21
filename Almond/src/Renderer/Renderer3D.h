#pragma once
#include "Renderer/Renderer.h"

class Renderer3D : public Renderer
{
private:
	std::vector<std::shared_ptr<Model>> m_Models;
	Model backpack;
public:
	Renderer3D();
	~Renderer3D() override;

	void Shutdown() override;
	void BeginScene(EditorCamera& camera) override;
	void EndScene() override;
	void Submit(const glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, int textureID,
		glm::vec2* texCoords) override;
	void ResetStats() override;
	RenderStats GetStats() override;
	void Submit(std::shared_ptr<Model> model) override;
};
