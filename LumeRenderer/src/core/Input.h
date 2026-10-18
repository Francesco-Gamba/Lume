#pragma once 
#include <array>
#include <glm/glm.hpp>

namespace lume {

	enum class Key : int
	{
    W, A, S, D, Q, E, F, LeftShift, LeftCtrl, LeftAlt, 
	Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, Count
	};


	enum class MouseButton : int
    {
		Left, Right, Middle, Count
	};

	struct InputState
	{
        glm::vec2 mouse_pos {0};
        glm::vec2 mouse_delta {0};
		float scroll = 0;

		std::array<bool, (int)Key::Count> key_down = {};
        std::array<bool, (int)Key::Count> key_pressed = {};

		std::array<bool, (int)MouseButton::Count> mouse_down = {};
        std::array<bool, (int)MouseButton::Count> mouse_pressed = {};

		bool down(Key k) const { return key_down[(int)k]; }
        bool pressed(Key k) const { return key_pressed[(int)k]; }
        bool down(MouseButton b) const { return mouse_down[(int)b]; }
        bool pressed(MouseButton b) const { return mouse_pressed[(int)b]; }

		void clear_mouse() {
            mouse_delta = glm::vec2(0.0f);
			scroll = 0;
            mouse_down = {};
            mouse_pressed = {};
		}

		void clear_keyboard() { 
			key_down = {};
            key_pressed = {};
		}
	};
}