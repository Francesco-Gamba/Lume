#pragma once
#include <glm/glm.hpp>

namespace lume {
	class Camera
	{
		public:

			glm::vec3 position{0, 0, 0};
			float yaw = 0;
            float pitch = 0;
			float fov_y = 60;
            float near_plane = 0.1f;
            float far_plane = 1000.0f;

			glm::vec3 forward() const;
            glm::vec3 right() const;
            glm::vec3 up() const;
            glm::mat4 view() const;
            glm::mat4 proj(float aspect) const;

			void lookAt(const glm::vec3& target);
            void clampPitch();
	};
}