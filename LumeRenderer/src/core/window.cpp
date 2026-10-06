#define GLFW_INCLUDE_NONE

#include "core/window.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <stdexcept>

namespace lume{

    void Window::error_callback(int error, const char* description) {
        std::cerr << error << " " << description << "\n";
    }

    void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    Window::Window(int width, int height, const char* title)
    {
        
        glfwSetErrorCallback(error_callback);

        if (glfwInit() != GLFW_TRUE) {
            throw std::runtime_error("glfw init failed");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        handle_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (handle_ == nullptr) {
            glfwTerminate();
            throw std::runtime_error("glfwCreateWindow failed: no OpenGL 4.6 core context available");
        }

        glfwSetKeyCallback(handle_, key_callback);
        glfwMakeContextCurrent(handle_);
        glfwSwapInterval(1);


        if (gladLoadGL(glfwGetProcAddress) == 0) {
            glfwDestroyWindow(handle_);
            glfwTerminate();
            throw std::runtime_error("gladLoadGl failed to load OpenGl functions");
        }
    }

    Window::~Window() {
        if (handle_ != nullptr) {
            glfwDestroyWindow(handle_);
        }
        glfwTerminate();
    }

    bool Window::should_close() const {
        return glfwWindowShouldClose(handle_);
    }

    void Window::swap_buffers() {
        glfwSwapBuffers(handle_);
    }

    void Window::poll_events() {
        glfwPollEvents();
    }

    void Window::framebuffer_size(int& width, int& height) const {
        glfwGetFramebufferSize(handle_, &width, &height);
    }
}