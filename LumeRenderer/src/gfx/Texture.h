#pragma once
#include <vector>
#include <glad/gl.h>
#include <cstdint>
#include <utility>

namespace lume {
	class Texture
	{
		private:
            GLuint handle_ = 0;

		public:

			explicit Texture(int width, int height, bool srgb, const std::vector<uint8_t>& pixels);
            ~Texture();

            Texture(const Texture&) = delete;
            Texture& operator=(const Texture&) = delete;

            Texture(Texture&& other) noexcept;
            Texture& operator=(Texture&& other) noexcept; 

			void bind(GLuint unit) const;

			
	};
}

