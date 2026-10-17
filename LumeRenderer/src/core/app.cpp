#include "core/app.h"
#include "gfx/gl_debug.h"
#include <glm/gtc/matrix_transform.hpp>   // translate, rotate, lookAt, perspective
#include <glm/gtc/type_ptr.hpp>           // value_ptr
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>                   // glfwGetTime
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>
#include "scene/Mesh_Loader.h"
#include "ui/debug_panel.h"
#include <imgui.h>


namespace lume {


	App::App() : window(1024, 768, "Lume"), imgui_(window.handle()), mesh_shader(LUME_SHADER_DIR "/mesh.glsl")
    {
        std::cout << glGetString(GL_VENDOR) << " | " << glGetString(GL_RENDERER) << " | " << glGetString(GL_VERSION) << "\n";
        install_gl_debug_output();



        for (const MeshData& data : load_mesh(LUME_ASSET_DIR "/meshes/Separated.glb")) {
            meshes_.emplace_back(data);
            scene_bounds_.expand(data.bounds);
        }

        std::cout << meshes_.size() << " Meshes loaded. \n";

        stats_.gpu_name = reinterpret_cast<const char*> (glGetString(GL_RENDERER));
        stats_.meshes = (int)meshes_.size();
        stats_.triangles = 0;
        
        for (const Mesh& mesh : meshes_) {
            stats_.triangles += mesh.index_count() / 3;
        }

        // Frame the camera on the model's bounding box
        glm::vec3 center = scene_bounds_.center();
        float radius = scene_bounds_.radius();
        float dist = radius / glm::sin(glm::radians(camera_.fov_y * 0.5f)) * 1.1f;
        camera_.position = center + glm::vec3(0, radius * 0.3f, dist);
        camera_.look_at(center);
        controller_.pivot_distance = glm::length(center - camera_.position);   // first orbit/zoom goes around the model
        camera_.near_plane = radius * 0.01f;
        camera_.far_plane = dist + radius * 4.0f;

        glEnable(GL_DEPTH_TEST);
	}

	void App::frame(){
        int width, height;
        window.framebuffer_size(width, height);
        if (width == 0 || height == 0) return;   // minimized window
        glViewport(0, 0, width, height);

        if (settings_.vsync != prev_vsync_) {
            glfwSwapInterval(settings_.vsync ? 1 : 0);
            prev_vsync_ = settings_.vsync;
        }

        glClearColor(settings_.clear_color.r, settings_.clear_color.g, settings_.clear_color.b, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        settings_.cull ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, settings_.wireframe ? GL_LINE : GL_FILL);

        glm::mat4 model = glm::mat4(1.0f);
        float aspect = float(width) / float(height);

        //Camera Buffer
        camera_ub_.update(camera_, aspect);

        //Shader
        mesh_shader.bind();
        mesh_shader.set("uModel", model);
        mesh_shader.set("uViewMode", (int)settings_.view);
        mesh_shader.set("uSunDir", settings_.sun_dir);

        stats_.draw_calls = 0;
        for (const Mesh& mesh : meshes_) {
            mesh.draw();
            ++ stats_.draw_calls;
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void App::run() {

        double last = glfwGetTime();

        while (!window.should_close()) {
            
            double now = glfwGetTime();
            delta_time_ = float(now - last);
            stats_.update(float(delta_time_ * 1000.0));
            last = now;



            imgui_.begin_frame();
            
            InputState in = window.input();
            const ImGuiIO& io = ImGui::GetIO();
            
            if (io.WantCaptureMouse) {
                in.clear_mouse();
            }

            if (io.WantCaptureKeyboard) {
                in.clear_keyboard();
            }

            int fb_w = 0, fb_h = 0;
            window.framebuffer_size(fb_w, fb_h);
            if (fb_h > 0) {                                        // minimized window: pan would divide by 0
                controller_.camera_update(in, camera_, delta_time_, float(fb_h));
            }
            draw_debug_panel(camera_, settings_ ,stats_, in, controller_);

            frame();
            
            imgui_.end_frame();

            window.swap_buffers();
            window.poll_events();
        
        }
	}
}
