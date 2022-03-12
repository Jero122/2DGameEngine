set(PROJECT_NAME Almond)

################################################################################
# Source groups
################################################################################
set(src
    "src/Almond.h"
)
source_group("src" FILES ${src})

set(src__Core
    "src/Core/AlmondApplication.cpp"
    "src/Core/AlmondApplication.h"
    "src/Core/Base.h"
    "src/Core/EntryPoint.h"
    "src/Core/Input.cpp"
    "src/Core/Input.h"
    "src/Core/Layer.cpp"
    "src/Core/Layer.h"
    "src/Core/LayerStack.cpp"
    "src/Core/LayerStack.h"
    "src/Core/TimeStep.h"
    "src/Core/Window.cpp"
    "src/Core/Window.h"
)
source_group("src\\Core" FILES ${src__Core})

set(src__ECS
    "src/ECS/ComponentManager.h"
    "src/ECS/ECS.h"
    "src/ECS/ECSTypes.cpp"
    "src/ECS/ECSTypes.h"
    "src/ECS/EntitiyManager.h"
    "src/ECS/Entity.h"
    "src/ECS/SceneView.h"
    "src/ECS/SystemManager.h"
)
source_group("src\\ECS" FILES ${src__ECS})

set(src__ECS__Components
    "src/ECS/Components/BoxCollider2D.h"
    "src/ECS/Components/RigidBody.h"
    "src/ECS/Components/SpriteRenderer.h"
    "src/ECS/Components/TagComponent.h"
    "src/ECS/Components/Transform.h"
)
source_group("src\\ECS\\Components" FILES ${src__ECS__Components})

set(src__Layers
    "src/Layers/ImGuiLayer.cpp"
    "src/Layers/ImGuiLayer.h"
    "src/Layers/InputLayer.h"
)
source_group("src\\Layers" FILES ${src__Layers})

set(src__Physics2D
    "src/Physics2D/DebugDrawBox2D.cpp"
    "src/Physics2D/DebugDrawBox2D.h"
    "src/Physics2D/Geometry2D.cpp"
    "src/Physics2D/Geometry2D.h"
    "src/Physics2D/Physics2D.cpp"
    "src/Physics2D/Physics2D.h"
)
source_group("src\\Physics2D" FILES ${src__Physics2D})

set(src__Platforms__OpenGL
    "src/Platforms/OpenGL/imgui_impl_opengl3.cpp"
    "src/Platforms/OpenGL/imgui_impl_sdl.cpp"
)
source_group("src\\Platforms\\OpenGL" FILES ${src__Platforms__OpenGL})

set(src__Platforms__Windows
    "src/Platforms/Windows/WindowsWindow.cpp"
    "src/Platforms/Windows/WindowsWindow.h"
)
source_group("src\\Platforms\\Windows" FILES ${src__Platforms__Windows})

set(src__Renderer
    "src/Renderer/Camera.h"
    "src/Renderer/CPUBatched_Renderer.cpp"
    "src/Renderer/CPUBatched_Renderer.h"
    "src/Renderer/EditorCamera.cpp"
    "src/Renderer/EditorCamera.h"
    "src/Renderer/GLCall.h"
    "src/Renderer/GPUBatched_Renderer.cpp"
    "src/Renderer/GPUBatched_Renderer.h"
    "src/Renderer/OpenGLBuffer.h"
    "src/Renderer/OpenGLRenderCommand.h"
    "src/Renderer/OpenGLVertexArray.h"
    "src/Renderer/Renderer.h"
    "src/Renderer/Shader.cpp"
    "src/Renderer/Shader.h"
    "src/Renderer/SpriteSheet.cpp"
    "src/Renderer/SpriteSheet.h"
    "src/Renderer/Texture.cpp"
    "src/Renderer/Texture.h"
)
source_group("src\\Renderer" FILES ${src__Renderer})

set(src__ResourceManager
    "src/ResourceManager/ResourceManager.cpp"
    "src/ResourceManager/ResourceManager.h"
)
source_group("src\\ResourceManager" FILES ${src__ResourceManager})

set(src__Scenes
    "src/Scenes/Scene.cpp"
    "src/Scenes/Scene.h"
    "src/Scenes/SceneSerializer.cpp"
    "src/Scenes/SceneSerializer.h"
)
source_group("src\\Scenes" FILES ${src__Scenes})

set(vendor__stb__stb
    "vendor/stb/stb/stb_image.cpp"
    "vendor/stb/stb/stb_image.h"
)
source_group("vendor\\stb\\stb" FILES ${vendor__stb__stb})

set(ALL_FILES
    ${src}
    ${src__Core}
    ${src__ECS}
    ${src__ECS__Components}
    ${src__Layers}
    ${src__Physics2D}
    ${src__Platforms__OpenGL}
    ${src__Platforms__Windows}
    ${src__Renderer}
    ${src__ResourceManager}
    ${src__Scenes}
    ${vendor__stb__stb}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Almond)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "Almond"
    TARGET_NAME_RELEASE "Almond"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/../bin/Debug-windows-x86_64/Almond/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../bin/Release-windows-x86_64/Almond/"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
        MultiThreadedDebugDLL
    >
    $<$<CONFIG:Release>:
        MultiThreadedDLL
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src;"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor;"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui;"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb;"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm;"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include/SDL;"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include/GL;"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include/box2d;"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/yaml-cpp/include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "ALM_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "ALM_RELEASE"
    ">"
    "ALM_PLATFORM_WINDOWS;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /ZI
        >
        $<$<CONFIG:Release>:
            /Ox;
            /Oi;
            /GF;
            /Gy;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        >
        /MP;
        /std:c++17;
        /Gm-;
        /W3;
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Y-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:WINDOWS
    )
endif()

################################################################################
# Dependencies
################################################################################
# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    ImGui
    yaml-cpp
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "glew32s;"
    "opengl32;"
    "box2d"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/Debug"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/Release"
    ">"
)

