#pragma once
#include <glm/glm.hpp>
#include <cstdint>

namespace lume {

	constexpr uint32_t NoTexture = UINT32_MAX;

	struct Material
	{
        glm::vec4 base_color_factor {1, 1, 1, 1};
        uint32_t base_color_image = NoTexture;
        bool double_sided = false;
	};

}