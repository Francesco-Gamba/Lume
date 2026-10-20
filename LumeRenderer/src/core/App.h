#pragma once
#include "core/Window.h"
#include <glm/glm.hpp>
#include "math/BBox.h"
#include <vector>
#include "ui/ImGuiLayer.h"
#include "core/DebugSettings.h"
#include "scene/Camera.h"
#include "core/CameraController.h"
#include "scene/Scene.h"
#include "renderer/Renderer.h"


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

    Window window_;
    ImGuiLayer imgui_;
    Renderer renderer_;
    Scene scene_;
    Camera camera_;
    CameraController controller_;
    DebugSettings settings_;
    FrameStats stats_;
};

}  // namespace lume