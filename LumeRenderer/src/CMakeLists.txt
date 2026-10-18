add_executable(lume
    main.cpp
    core/Window.cpp
    core/App.cpp
    gfx/GlDebug.cpp
    gfx/Shader.cpp
    gfx/Mesh.cpp
    gfx/CameraUB.cpp
    scene/MeshLoader.cpp
    scene/Camera.cpp
    ui/ImGuiLayer.cpp
    ui/DebugPanel.cpp
    core/CameraController.cpp
    scene/Scene.cpp
)

target_include_directories(lume PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lume PRIVATE
    glad
    glfw
    glm::glm
    imgui::imgui
    assimp::assimp
)

if(MSVC)
    target_compile_options(lume PRIVATE /W4 /permissive- /Zc:preprocessor /utf-8)
endif()

target_compile_definitions(lume PRIVATE LUME_ASSET_DIR="${CMAKE_SOURCE_DIR}/assets")
target_compile_definitions(lume PRIVATE LUME_SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders")
