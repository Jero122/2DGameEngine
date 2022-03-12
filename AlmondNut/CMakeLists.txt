set(PROJECT_NAME AlmondNut)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "src/AlmondNutApp.cpp"
    "src/EditorLayer.cpp"
    "src/EditorLayer.h"
)
source_group("" FILES ${no_group_source_files})

set(Panels
    "src/Panels/SceneHierarchyPanel.cpp"
    "src/Panels/SceneHierarchyPanel.h"
)
source_group("Panels" FILES ${Panels})

set(ALL_FILES
    ${no_group_source_files}
    ${Panels}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE AlmondNut)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "AlmondNut"
    TARGET_NAME_RELEASE "AlmondNut"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/../bin/Debug-windows-x86_64/AlmondNut/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../bin/Release-windows-x86_64/AlmondNut/"
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
    "${CMAKE_CURRENT_SOURCE_DIR}/../Almond/src;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Almond/vendor;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Almond/dependencies/include/SDL;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Almond/dependencies/include/GL;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Almond/vendor/glm;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Almond/dependencies/include/box2d;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Almond/vendor/imgui"
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
        $<$<CONFIG:Debug>:
            /DEBUG;
            /INCREMENTAL
        >
        $<$<CONFIG:Release>:
            /DEBUG:FULL;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /SUBSYSTEM:CONSOLE
    )
endif()

################################################################################
# Dependencies
################################################################################
# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Almond
    ImGui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "glew32s;"
    "glu32;"
    "opengl32;"
    "box2d"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../Almond/dependencies/lib/Debug"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../Almond/dependencies/lib/Release"
    ">"
)

