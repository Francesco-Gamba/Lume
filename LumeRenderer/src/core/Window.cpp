#define GLFW_INCLUDE_NONE

#include "core/Window.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "core/Input.h"
#include <array>
#include <iostream>
#include <stdexcept>

namespace lume{

    namespace{
        constexpr std::array<int, static_cast<size_t>(Key::Count)> kGlfwKey = {
            GLFW_KEY_W,        GLFW_KEY_A,          GLFW_KEY_S,
            GLFW_KEY_D,        GLFW_KEY_Q,          GLFW_KEY_E,
            GLFW_KEY_F,        GLFW_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_CONTROL,
            GLFW_KEY_LEFT_ALT, GLFW_KEY_1,          GLFW_KEY_2,
            GLFW_KEY_3,        GLFW_KEY_4,          GLFW_KEY_5,
            GLFW_KEY_6,        GLFW_KEY_7,          GLFW_KEY_8,
            GLFW_KEY_9,
        };

        constexpr std::array<int, static_cast<size_t>(MouseButton::Count)> kGlfwButton = {
            GLFW_MOUSE_BUTTON_LEFT,
            GLFW_MOUSE_BUTTON_RIGHT,
            GLFW_MOUSE_BUTTON_MIDDLE,
        };
    }

    void Window::errorCallback(int error, const char* description) {
        std::cerr << error << " " << description << "\n";
    }

    void Window::keyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    void Window::scrollCallback(GLFWwindow* window, double /*xoff*/, double yoff) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->scroll_accum_ += static_cast<float>(yoff);
    }

    void Window::updateInput() {
        for (size_t i = 0; i < kGlfwKey.size(); ++i) {
            const bool now = glfwGetKey(handle_, kGlfwKey[i]) == GLFW_PRESS;
            input_.key_pressed[i] = now && !input_.key_down[i];
            input_.key_down[i] = now;
        }

        for (size_t i = 0; i < kGlfwButton.size(); ++i) {
            const bool now = glfwGetMouseButton(handle_, kGlfwButton[i]) == GLFW_PRESS;
            input_.mouse_pressed[i] = now && !input_.mouse_down[i];
            input_.mouse_down[i] = now;
        }

        double x = 0.0, y = 0.0;
        glfwGetCursorPos(handle_, &x, &y);
        const glm::vec2 pos(static_cast<float>(x), static_cast<float>(y));

        input_.mouse_delta = has_prev_mouse_ ? pos - prev_mouse_ : glm::vec2(0.0f);
        input_.mouse_pos = pos;
        prev_mouse_ = pos;
        has_prev_mouse_ = true;

        input_.scroll = scroll_accum_;
        scroll_accum_ = 0.0f;
    }

    Window::Window(int width, int height, const char* title)
    {
        
        glfwSetErrorCallback(errorCallback);


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

        glfwSetWindowUserPointer(handle_, this);  
        glfwSetKeyCallback(handle_, keyCallback);
        glfwSetScrollCallback(handle_, scrollCallback);
        glfwMakeContextCurrent(handle_);


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

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(handle_);
    }

    void Window::swapBuffers() {
        glfwSwapBuffers(handle_);
    }

    void Window::pollEvents() {
        glfwPollEvents();
        updateInput();
    }

    void Window::framebufferSize(int& width, int& height) const {
        glfwGetFramebufferSize(handle_, &width, &height);
    }
}