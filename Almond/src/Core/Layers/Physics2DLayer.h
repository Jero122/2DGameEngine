#pragma once
#include "Core/Layer.h"

class Physics2DLayer: public Layer
{
public:
	Physics2DLayer();
	~Physics2DLayer() override;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void OnLateUpdate() override;
};
