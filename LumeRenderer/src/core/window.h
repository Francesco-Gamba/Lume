#pragma once
#include "core/input.h"

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

    bool should_close() const;
    void swap_buffers();
    void poll_events();

    void framebuffer_size(int& width, int& height) const;

    GLFWwindow* handle() const { return handle_; }

    const InputState& input() const { return input_; } 

private:
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoff, double yoff);
    
    GLFWwindow* handle_ = nullptr;

    void update_input();
    InputState input_;
    float scroll_accum_ = 0;
    glm::vec2 prev_mouse_ {0};
    bool has_prev_mouse_ = false;

};

}  // namespace lume