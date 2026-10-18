#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace lume {
	struct Node
	{
        uint32_t mesh_id;
        glm::vec3 translation;
        glm::quat rotation;
        glm::vec3 scale;
	};
}