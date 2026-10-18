#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cstdint>
#include <glm/gtc/matrix_transform.hpp>


namespace lume {

    constexpr uint32_t NoMesh = UINT32_MAX;

	struct Node
	{
        uint32_t mesh_id = NoMesh;
        glm::vec3 translation {0.0f, 0.0f, 0.0f};
        glm::quat rotation = glm::identity<glm::quat>();
        glm::vec3 scale {1.0f, 1.0f, 1.0f};

        
        glm::mat4 localMatrix() const {
            glm::mat4 t = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 r = glm::mat4_cast(rotation);
            glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);

            return t*r*s;
        }
	};




}