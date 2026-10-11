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
        center_ = scene_bounds_.center();
        radius_ = scene_bounds_.radius();
        dist_   = radius_ / glm::sin(glm::radians(22.5f)) * 1.1f;   // fits a 45° vertical FOV
        eye_    = center_ + glm::vec3(0.0f, radius_ * 0.3f, dist_);

        glEnable(GL_DEPTH_TEST);
	}

	void App::frame(){
        int width, height;
        window.framebuffer_size(width, height);
        if (width == 0 || height == 0) return;   // minimized window

        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Matrices: spin the model around its own center
        float t = (float)glfwGetTime();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), center_) * glm::rotate(glm::mat4(1.0f), t * 0.5f, glm::vec3(0, 1, 0)) * glm::translate(glm::mat4(1.0f), -center_);
        glm::mat4 view  = glm::lookAt(eye_, center_, glm::vec3(0, 1, 0));
        glm::mat4 proj  = glm::perspective(glm::radians(35.0f), (float)width / height, radius_ * 0.01f, dist_ + radius_ * 4.0f);

        // Shader + uniforms
        mesh_shader.bind();
        mesh_shader.set("uModel", model);
        mesh_shader.set("uViewProj", proj * view);

        stats_.draw_calls = 0;
        for (const Mesh& mesh : meshes_) {
            mesh.draw();
            ++ stats_.draw_calls;
        }
	}

	void App::run() {

        double last = glfwGetTime();

        while (!window.should_close()) {
            
            double now = glfwGetTime();
            stats_.update(float((now-last) * 1000.0));
            last = now;


            imgui_.begin_frame();
            draw_debug_panel(stats_);

            frame();
            
            imgui_.end_frame();

            window.swap_buffers();
            window.poll_events();
        
        }
	}
}
