#pragma once
#include "core/Input.h"

struct GLFWwindow;

namespace lume {

class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();

    void framebufferSize(int& width, int& height) const;

    GLFWwindow* handle() const { return handle_; }

    const InputState& input() const { return input_; } 

private:
    static void errorCallback(int error, const char* description);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xoff, double yoff);
    
    GLFWwindow* handle_ = nullptr;

    void updateInput();
    InputState input_;
    float scroll_accum_ = 0;
    glm::vec2 prev_mouse_ {0};
    bool has_prev_mouse_ = false;

};

}  // namespace lume