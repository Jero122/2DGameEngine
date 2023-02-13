# Almond [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](/LICENSE)

Almond is 2D and 3D interactive application, rendering and game engine primarily for Windows. It is heavily inspired by the Cherno's Hazel2D but branches off into it's own direction, for example it has a custom made ECS rather than using Entt, and has a 3D pipeline, which Hazel2D does not have yet.

***
![Almond](/Resources/Branding/Editor.png)

## Getting Started
Visual Studio 2019 or 2022 is highly recommended, Almond is untested on any other development environment while I focus on the windows build first.

<ins>**1. Cloning the repository:**</ins>

Start by cloning the repository with:

```console
$ git clone --recursive https://github.com/Jero122/2DGameEngine
```
If the repository was cloned non-recursively previously, clone and update the neceesary submodules with:
```console
$ git submodule init
$ git submodule update
```

<ins>**2. Generating project files:**</ins>

Almond is built using **Premake 5**, as such this project comes with the prebuilt binaries of premake in vendor/premake and there is no need to download Premake. Generating the project files is as simple as running the [GenerateProjects.bat](/GenerateProjects.bat) file found in the root folder. This will setup the visual studio environments and create debug/release build directories under 'bin' with all the required libraries and assets.



<ins>**3. Downloading the assets:**</ins>

Some of the assets used to test the project can be very large, hence they are not included with the repo. Download the assets from this [dropbox link](https://www.dropbox.com/sh/hafq9ud29nb8j27/AACu-dJNOZ0OD6EloWcOhRqVa?dl=0), unzip the file and place the **assets folder** in the [AlmondNut directory](/AlmondNut/)

<ins>**4. Building the engine and editor:**</ins>

If the previous steps have followed, a Visual Studio Solution (.sln) file will be generated in the root directory. Simply open the Visual Studio Solution and build the engine as either debug or release mode.
***

## Scripting

### **Creating new entities:**

Currently the engine has no seperate scripting engine and scene management is still WIP. Hence, all scripting is done in either [EditorSystem.cpp](/AlmondNut/src/EditorSystem.cpp) or [Scene.cpp](Almond/src/Scenes/Scene.cpp) for now. The EditorSystem is a subsystem that sits ontop of Almond Engine's **System Stack** and is directly responsible for the AlmondNut Application. Temporarily, it also holds the reference to the current game scene so any entities that need to be added can be added here, alternatively they can be added in the constructor for Scene in [Scene.cpp](Almond/src/Scenes/Scene.cpp). For example to create a helmet object, place this snippet in the **EditorSystem::OnStart() function**:

```c++
Entity damagedHelmet = m_CurrentScene->CreateEntity("damaged helmet");
    {
        //Gets the transform component from the entity (every entitiy has a transform component)
        auto transformComponent = damagedHelmet.GetComponent<Transform>();
        //Changes the actaul value of the transform to a new position, rotation and translation
        *transformComponent = Transform{ glm::vec3(3,0,0), glm::vec3(90,0,0),glm::vec3(1.5f,1.5f,1.5f) };
        //Creates a pointer to a Model(Model ref counting is not availible yet)
        auto hetlmetMesh = std::make_shared<Model>("assets/Models/DamagedHelmet/DamagedHelmet.gltf");
        //Adds a ModelRendererComponent to the helmet entity with a reference to the Model
        damagedHelmet.AddComponent(ModelRendererComponent{ hetlmetMesh });
    }
```

### **Creating new components:**

<ins>**1. Create the component file:**</ins>

Create a new desired component as a struct. Remember components have no logic so just include the data, any constructors and/or getters. For example:

```c++
struct MovementComponent
{
public:
	float speed;
};
```

<ins>**2. Register the component:**</ins>

Once the component has been created, you need to register the component in the [Scene.cpp](/Almond/src/Scenes/Scene.cpp) Scene() constructor:

```c++
Scene::Scene()
{
	m_Ecs.Init();
	m_Ecs.CreateComponent<MovementComponent>();
}
```
<ins>**3. Implementing Logic:**</ins>

Logic can be implemented in [Scene](Almond/src/Scenes/Scene.cpp)'s **OnRuntimeUpdate** method:
```c++
	/*
	SceneView<Transform, MovementComponent> gets all entities in the scene with BOTH these components
	SceneView<MovementComponent> would still work as ALL entities have a transform component
	*/
	for (EntityID ent : SceneView<Transform, MovementComponent>(m_Ecs))
	{
		//Get the components from the entity
		auto transform = m_Ecs.GetComponent<Transform>(ent);
		auto move = m_Ecs.GetComponent<MovementComponent>(ent);
		//increment the y position according to the movement component's speed
		transform->position.y -= move->speed * timestep.GetSeconds();
	}
```
<ins>**4. Limitations:**</ins>

The scripting capabilities of the engine is currently quite limited, as all scripting has to happen in the Scene class and currently there is no way to change scenes. In the future, the logic implementation would be in seperate files, that would get picked up automatically by the current scene and executed. However, as this is a one-man engine, this may take time.

However, the current system is more than enough to create engine subsystems. For example the renderer works in the following way:

```c++
void Scene::Render(EditorCamera& editorCamera)
{
	//Submit all Model Components
	m_Renderer3D->BeginScene(editorCamera);
	for (EntityID ent : SceneView<Transform, ModelRendererComponent>(m_Ecs))
	{
		auto transform = m_Ecs.GetComponent<Transform>(ent);
		auto modelRenderer = m_Ecs.GetComponent<ModelRendererComponent>(ent);
		m_Renderer3D->Submit(modelRenderer->model, transform->position, transform->rotation, transform->scale, ent);
	}

	//Submit all MeshComponents
	for (EntityID ent : SceneView<Transform, MeshRendererComponent>(m_Ecs))
	{
		auto transform = m_Ecs.GetComponent<Transform>(ent);
		auto meshRenderer = m_Ecs.GetComponent<MeshRendererComponent>(ent);
		m_Renderer3D->Submit(meshRenderer->glMesh, transform->position, transform->rotation, transform->scale, ent);
	}

	//Submit all Lights
	for (EntityID entt : SceneView<Transform, LightComponent>(m_Ecs))
	{
		auto transform = m_Ecs.GetComponent<Transform>(entt);
		auto light = m_Ecs.GetComponent<LightComponent>(entt);
		if (light->type == LightComponent::PointLight)
		{
			m_Renderer3D->SubmitPointLight(transform->position, light->ambient, light->diffuse, light->specular, light->constant, light->linear, light->quadratic);
		}
		else if (light->type == LightComponent::SpotLight)
		{
			m_Renderer3D->SubmitSpotLight(transform->position, light->ambient, light->diffuse, light->specular, light->direction, light->innerCutOff, light->outerCutoff);
		}
		else if (light->type == LightComponent::DirectionLight)
		{
			m_Renderer3D->SetDirectionalLight(light->ambient, light->diffuse, light->specular, light->direction);
		}
	}
	m_Renderer3D->EndScene();
}
```

The Render function is called each frame by the OnRuntimeUpdate/OnEditorUpdate function. The data-oriented ECS design allows a list of all Model/Mesh/Light components to be traversed and submitted really quickly due to the underlying nature of how each component is stored: in a tightly packed cache friendly list.
***
## Engine Features
- **Custom Entity Component System Architecture**
    - Able to handle a *large* number of entities
    - Performant, maximises cache locality
- **2D Renderer**
    - Batched Renderer, able to render 100s of thousands of sprites
    - Support for textures and spritesheets
    - Multithreaded batching availible, fully utilises the CPU to compute multiple batches in parrallel
    - **Currently not working in the current build due to the ongoing refactoring of the 3D pipeline**
- **3D Renderer**
    - Physically Based rendering
    - Custom PBR material system, easy to define new materials
    - Complete support for GLTF 2.0
    - Custom Binary format for models, auto generated when a new model is discovered in the filesystem
    - Image based reflections/ Skybox support
    - Shaddow mapping (WIP)
    - Post processing (WIP)
- **Scene management and serializing**
    - Scenes can be serialised, currently into YAML files
    - Scene Loading and binary serialisation (WIP)
-  **Reference counting Texture and Material Loading system**
-  **Logging macros**
***
## Almond Nut - The Editor
- **Unity/Unreal inspired editor**
- **Asset Browser**
    - Custom file-system
        - File tree with a file watcher, updated if change detected
        - Auto file generation (currently converts new GLTF models into custom binary format)
    - String/sub string search
    - Resizeable icon and tree view
    - AwesomeFont support - uses IconFont instead of sprites to save resources
- **Properties Panel**
    - Displays the list of components belonging to the selected entity
    - Real-time editiing of components
    - Easy to add panels for components
- **Viewport**
    - Pixel perfect mouse-picking, no ray casting
    - Translate, rotate and scale tools, with appropriate visual gizmos
    - Viewport camera can pan, rotate, zoom
- **Scene Hierarchy**
    - Displays list of all entities in current scene
    - Allows selection of enetities


## Short term goals:
- High-fidelity Physically-Based 3D rendering (WIP)
    - Scene graph, Opaque sorting and frustum culling
    - Skeletal Animations
    - LOD support
- Merge the 2D renderer with the 3D renderer 
- CPU Particle system
- Improved Editor
    - Previews for models and textures
    - Drag and drop
- 3rd Party 3D physics such as Bullet or PhysX
- Write some documentation...

## Long term goals :
- Seperate, hot reloadable C++ scripting engine
- Raytraced reflections/shadows/AO and Global illumination
- Support for projects and build process for games
- Game UI framework

