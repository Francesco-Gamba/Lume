#pragma once
#include "core/Window.h"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include "gfx/Shader.h"
#include "gfx/Mesh.h"
#include "math/BBox.h"
#include <vector>
#include "ui/ImGuiLayer.h"
#include "core/DebugSettings.h"
#include "scene/Camera.h"
#include "core/CameraController.h"
#include "gfx/CameraUB.h"


namespace lume {

class App
{
public:
    App();

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    void run();

private:
    void frame();
    float delta_time_ = 0.0f;

    Window window;
    CameraUB camera_ub_;
    ImGuiLayer imgui_;
    Shader mesh_shader;
    FrameStats stats_;
    DebugSettings settings_;
    bool prev_vsync_ = !DebugSettings{}.vsync;

    std::vector<Mesh> meshes_;
    BBox scene_bounds_;
    
    Camera camera_;
    CameraController controller_;
};

}  // namespace lume