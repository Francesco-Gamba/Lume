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

    std::vector<Mesh> meshes_;
    BBox scene_bounds_;

    glm::vec3 center_{0.0f};
    glm::vec3 eye_{0.0f};
    
    float radius_ = 1.0f;
    float dist_ = 1.0f;
};

}  // namespace lume