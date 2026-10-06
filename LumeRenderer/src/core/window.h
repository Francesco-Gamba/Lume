#pragma once

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

private:
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    GLFWwindow* handle_ = nullptr;
};

}  // namespace lume