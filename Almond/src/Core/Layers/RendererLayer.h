#pragma once
#include "Core/Layer.h"

class RendererLayer: public Layer
{
public:
	RendererLayer();
	~RendererLayer() override;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void OnLateUpdate() override;
};

