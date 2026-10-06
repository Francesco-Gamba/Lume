#pragma once
#include "core/window.h"
#include <glad/gl.h>
#include <glm/glm.hpp>


namespace lume {

	class App
	{
    public:
        App();

        App(const App&) = delete;
        App& operator=(const App&) = delete;

        void run();

    private:
        void frame();
        Window window;


        GLuint vao;
        GLuint vbo;
        GLuint veo;
        GLuint program;
        GLsizei index_count = 0;

        glm::vec3 center_{0.0f};
        glm::vec3 eye_{0.0f};
        float radius_ = 1.0f;
        float dist_ = 1.0f;

	};

} //namespace lume