#pragma once

struct GLFWwindow;

namespace lume {


	class ImGuiLayer
	{
    public:
        explicit ImGuiLayer(GLFWwindow* window);
        ~ImGuiLayer();

        ImGuiLayer(const ImGuiLayer&) = delete;
        ImGuiLayer& operator=(const ImGuiLayer&) = delete;

        ImGuiLayer(ImGuiLayer&& other) = delete;
        ImGuiLayer& operator=(ImGuiLayer&& other) = delete;

        void begin_frame();
        void end_frame();
	};
}