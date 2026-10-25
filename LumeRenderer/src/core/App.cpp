#include "core/App.h"
#include <glm/gtc/matrix_transform.hpp>   // translate, rotate, lookAt, perspective
#include <glm/gtc/type_ptr.hpp>           // value_ptr
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>                   // glfwGetTime
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>
#include "ui/DebugPanel.h"
#include <imgui.h>



namespace lume {


	App::App() : window_(1024, 768, "Lume"), imgui_(window_.handle())
    {
        window_.setVsync(settings_.vsync);

        renderer_.loadScene(std::filesystem::path(LUME_ASSET_DIR) / "meshes/DamagedHelmet.glb", scene_);
        stats_.gpu_name = reinterpret_cast<const char*> (glGetString(GL_RENDERER));
        stats_.meshes = renderer_.meshCount();
        stats_.triangles = renderer_.triangleCount();

        glm::vec3 center = {0, 0, 0};
        float radius = 1;

        // Frame the camera on the model's bounding box
        if (scene_.getSceneBounds().isValid()) {
            center = scene_.getSceneBounds().center();
            radius = scene_.getSceneBounds().radius();
        }

        float dist = radius / glm::sin(glm::radians(camera_.fov_y * 0.5f)) * 1.1f;
        camera_.position = center + glm::vec3(0, radius * 0.3f, dist);
        camera_.lookAt(center);
        controller_.pivot_distance = glm::length(center - camera_.position);
        camera_.near_plane = radius * 0.01f;
        camera_.far_plane = dist + radius * 4.0f;
    }

	void App::frame(){
        int width, height;
        window_.framebufferSize(width, height);
        if (width == 0 || height == 0)
            return;  // minimized 

        scene_.updateTransforms();   // world matrices for anything edited this frame
        renderer_.render(scene_, camera_, settings_, width, height, stats_);
	}

	void App::run() {

        double last = glfwGetTime();

        while (!window_.shouldClose()) {
            
            double now = glfwGetTime();
            delta_time_ = float(now - last);
            stats_.update(float(delta_time_ * 1000.0));
            last = now;

            imgui_.beginFrame();
            
            InputState in = window_.input();
            const ImGuiIO& io = ImGui::GetIO();
            
            if (io.WantCaptureMouse) {
                in.clearMouse();
            }

            if (io.WantCaptureKeyboard) {
                in.clearKeyboard();
            }

            int fb_w = 0, fb_h = 0;
            window_.framebufferSize(fb_w, fb_h);
            if (fb_h > 0) {
                controller_.cameraUpdate(in, camera_, delta_time_, float(fb_h));
            }


            drawDebugPanel(camera_, settings_, stats_, in, controller_, scene_);

            frame();
            
            imgui_.endFrame();



            if (settings_.vsync != window_.getVsync()) {
                window_.setVsync(settings_.vsync);
            }
                        
            window_.swapBuffers();
            window_.pollEvents();
        
        }
	}
}
