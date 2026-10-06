#include <glad/gl.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <iostream>
#include <cstdlib>
#include <core/window.h>


// Ask hybrid-graphics drivers for the discrete GPU.

extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}


int main(){	

    try {
        lume::Window window(1024, 720, "Lume");

        while (!window.should_close()) {
            int width, height;
            window.framebuffer_size(width, height);

            glViewport(0, 0, width, height);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            window.swap_buffers();
            window.poll_events();
        }
    }

    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}