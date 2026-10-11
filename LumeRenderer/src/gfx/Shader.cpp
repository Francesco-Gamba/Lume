#include "gfx/shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace lume {

    namespace {
        std::string read_file(const std::filesystem::path& path){
    
            std::ifstream file(path, std::ios::binary);
            if (!file){
                throw std::runtime_error("Cannot open shader: " + path.string());
            }

            std::stringstream ss;
            ss << file.rdbuf();
            return ss.str();
        }


        void split_stages(const std::string& file, std::string& vertex, std::string& fragment) {
            std::istringstream in(file);
            std::string line;
            std::string common;
            std::string* current = &common;
            int line_no = 0;

            while (std::getline(in, line)) {
                line_no++;
                if (line.starts_with("#stage vertex")) {
                    vertex = common + "#line " + std::to_string(line_no + 1) + "\n";
                    current = &vertex;
                    continue;
                }

                if (line.starts_with("#stage fragment")) {
                    fragment = common + "#line " + std::to_string(line_no + 1) + "\n";
                    current = &fragment;
                    continue;
                }

                *current += line + "\n";
            }
        }

        GLuint compile_stage(GLenum stage,
            const std::string& source,
            const std::filesystem::path& path,
            const char* stage_name)
        {
            const char* src = source.c_str();
            
            GLuint shader = glCreateShader(stage);
            glShaderSource(shader, 1, &src, nullptr);
            glCompileShader(shader);

            GLint ok = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
            if (!ok) {
                GLint len = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
                std::string log(len, '\0');
                glGetShaderInfoLog(shader, len, nullptr, log.data());
                glDeleteShader(shader);
                throw std::runtime_error(path.string() + " [" + stage_name + " ]\n" + log);
            }
            return shader;
        }


    
    }


    Shader::Shader(const std::filesystem::path& path) {
        std::string vs_src;
        std::string fs_src;
        split_stages(read_file(path), vs_src, fs_src);

        if (vs_src.empty() || fs_src.empty()) {
            throw std::runtime_error(path.string() + ": missing #stage vertex or #stage fragment");
        }

        // If this throws, the exception already carries the compiler log.
        GLuint vs = compile_stage(GL_VERTEX_SHADER, vs_src, path, "vertex");
        GLuint fs = 0;

        try {
            fs = compile_stage(GL_FRAGMENT_SHADER, fs_src, path, "fragment");
        }
        catch (...) {
            glDeleteShader(vs);  // destructor won't run if the constructor throws
            throw;               // rethrow the original exception, log included
        }


        program_ = glCreateProgram();
        glAttachShader(program_, vs);
        glAttachShader(program_, fs);
        glLinkProgram(program_);
        glDeleteShader(vs);
        glDeleteShader(fs);

        GLint ok = 0;
        glGetProgramiv(program_, GL_LINK_STATUS, &ok);

        if (!ok) {
            GLint len = 0;
            glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &len);
            std::string log(len, '\0');
            glGetProgramInfoLog(program_, len, nullptr, log.data());
            glDeleteProgram(program_);
            program_ = 0;
            throw std::runtime_error(path.string() + " [link]\n" + log);
        }
    }

    Shader::~Shader()
    {
        if (program_ != 0) {
            glDeleteProgram(program_);
        }
    }

    void Shader::bind() const {
        glUseProgram(program_);
    }

}
