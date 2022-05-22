#pragma once
#include <string>

#include "Renderer/Model.h"

struct ModelRenderer
{
	std::shared_ptr<Model> model;

	explicit ModelRenderer(const std::shared_ptr<Model>& model)
		: model(model)
	{
	}

	ModelRenderer() = default;
};
