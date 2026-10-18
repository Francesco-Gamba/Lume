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

        void beginFrame();
        void endFrame();
	};
}