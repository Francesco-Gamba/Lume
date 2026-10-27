#include "gfx/Texture.h"
#include <cassert>
#include <cmath>
#include <algorithm>

namespace lume {

	
	Texture::Texture(int width, int height, bool srgb, const std::vector<uint8_t>& pixels) {
        assert(pixels.size() == size_t(width) * height * 4);

		const int levels = static_cast<int>(1 + std::floor(std::log2f(std::max(width, height))));
        const GLenum internal_format = srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;

		glCreateTextures(GL_TEXTURE_2D, 1, &handle_);
        glTextureStorage2D(handle_, levels, internal_format, width, height);
        glTextureSubImage2D(handle_, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
        glTextureParameteri(handle_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(handle_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(handle_, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(handle_, GL_TEXTURE_WRAP_T, GL_REPEAT);

        static const GLfloat max_aniso = [] {
            GLfloat v = 1.0f;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &v);
            return v;
        }();
        glTextureParameterf(handle_, GL_TEXTURE_MAX_ANISOTROPY, std::min(16.0f, max_aniso));

        glGenerateTextureMipmap(handle_);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &handle_);
        handle_ = 0;
    }

    Texture::Texture(Texture&& other) noexcept {
        handle_ = other.handle_;
        other.handle_ = 0;
    }

    Texture& Texture::operator=(Texture&& other) noexcept
    {
        if (this != &other) {
            glDeleteTextures(1, &handle_);
            handle_ = std::exchange(other.handle_, 0);
        }

        return *this;
    }

    void Texture::bind(GLuint unit) const {
        glBindTextureUnit(unit, handle_);
    }

}  // namespace lume
