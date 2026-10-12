#pragma once
#include "core/window.h"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include "gfx/shader.h"
#include "gfx/Mesh.h"
#include "math/BBox.h"
#include <vector>
#include "ui/imgui_layer.h"
#include "core/debug_settings.h"
#include "scene/Camera.h"


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

    Window window;
    ImGuiLayer imgui_;
    Shader mesh_shader;
    FrameStats stats_;
    DebugSettings settings_;
    bool prev_vsync_ = !DebugSettings{}.vsync;   // forced mismatch: frame 1 always applies the setting

    std::vector<Mesh> meshes_;
    BBox scene_bounds_;
    
    Camera camera_;
};

}  // namespace lume