#pragma once
#include "Core/AlmondApplication.h"


/// ECS ///
#include "ECS/Components/Transform.h"
#include "ECS/Components/TagComponent.h"
#include "ECS/Components/SpriteRenderer.h"
#include "ECS/Components/RigidBody.h"
#include "ECS/ECS.h"
#include "ECS/ECSTypes.h"
//Maybe dont need these
#include "ECS/EntitiyManager.h"
#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"
#include "ECS/Entity.h"
#include "Scenes/Scene.h"
#include "ECS/SceneView.h"

/// Core ///
#include "Core/Input.h"
#include "Core/GameSystem.h"
#include "Core/TimeStep.h"
#include "Core/Log.h"

/// RENDERER ///
#include "Renderer/Camera.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/EditorCamera.h"
#include "Renderer/GLCall.h" //TODO GLCALL shouldnt be here
#include "Renderer/SpriteSheet.h"