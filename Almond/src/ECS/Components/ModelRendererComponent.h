#pragma once
#include <string>
#include "Renderer/Model.h"

struct ModelRendererComponent
{
	std::shared_ptr<Model> model;

	explicit ModelRendererComponent(const std::shared_ptr<Model>& model)
		: model(model)
	{
	}

	ModelRendererComponent() = default;
};
