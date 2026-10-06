#include "gfx/gl_debug.h"

#include <glad/gl.h>

#include <iostream>

namespace lume {

    namespace {
        void GLAD_API_PTR gl_debug_callback(GLenum source,
                                            GLenum type,
                                            GLuint id,
                                            GLenum severity,
                                            GLsizei /*length*/,
                                            const GLchar* message,
                                            const void* /*user_param*/)
        {
            const char* src = "other";
            switch (source) {
            case GL_DEBUG_SOURCE_API: src = "api"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: src = "shader"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: src = "window"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY: src = "third-party"; break;
            case GL_DEBUG_SOURCE_APPLICATION: src = "app"; break;
            }

            const char* kind = "other";
            switch (type) {
            case GL_DEBUG_TYPE_ERROR: kind = "ERROR"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: kind = "deprecated"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: kind = "UNDEFINED"; break;
            case GL_DEBUG_TYPE_PORTABILITY: kind = "portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE: kind = "perf"; break;
            case GL_DEBUG_TYPE_MARKER: kind = "marker"; break;
            }

            const char* sev = "notification";
            switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH: sev = "high"; break;
            case GL_DEBUG_SEVERITY_MEDIUM: sev = "medium"; break;
            case GL_DEBUG_SEVERITY_LOW: sev = "low"; break;
            }

            std::cerr << "[GL " << kind << "] " << src << " / " << sev << " (" << id << "): " << message << "\n";

        #ifdef _WIN32
            if (severity == GL_DEBUG_SEVERITY_HIGH) {
                __debugbreak();  // with SYNCHRONOUS on, the stack here is the offending call
            }
        #endif
        }
    }

    void install_gl_debug_output()
    {
        GLint context_flags = 0;
        glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);

        if ((context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) == 0) {
            std::cerr << "warning: no debug context, GL errors will be silent\n";
            return;
        }

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_callback, nullptr);
        glDebugMessageControl(
            GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    }

}  // namespace lume
