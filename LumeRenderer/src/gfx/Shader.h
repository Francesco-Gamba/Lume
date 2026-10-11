#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <type_traits>


namespace lume {

    class Shader
    {
        public:
            explicit Shader(const std::filesystem::path& path);
            ~Shader();

            Shader(const Shader&) = delete;
            Shader& operator=(const Shader&) = delete;

            void bind() const;

            GLuint handle() const { return program_; }

            template<typename T>
            void set(const char* name, const T& v) const
            {
                const GLint loc = location(name);

                if constexpr (std::is_same_v<T, int>)            glProgramUniform1i(program_, loc, v);
                else if constexpr (std::is_same_v<T, float>)     glProgramUniform1f(program_, loc, v);
                else if constexpr (std::is_same_v<T, glm::vec2>) glProgramUniform2fv(program_, loc, 1, glm::value_ptr(v));
                else if constexpr (std::is_same_v<T, glm::vec3>) glProgramUniform3fv(program_, loc, 1, glm::value_ptr(v));
                else if constexpr (std::is_same_v<T, glm::vec4>) glProgramUniform4fv(program_, loc, 1, glm::value_ptr(v));
                else if constexpr (std::is_same_v<T, glm::mat3>) glProgramUniformMatrix3fv(program_, loc, 1, GL_FALSE, glm::value_ptr(v));
                else if constexpr (std::is_same_v<T, glm::mat4>) glProgramUniformMatrix4fv(program_, loc, 1, GL_FALSE, glm::value_ptr(v));
                else static_assert(sizeof(T) == 0, "Unsupported uniform type");
            }

        private:
            GLint location(const char* name) const { return glGetUniformLocation(program_, name); }

            GLuint program_ = 0;
    };

}  // namespace lume
