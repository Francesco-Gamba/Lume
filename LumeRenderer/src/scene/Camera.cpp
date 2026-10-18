#include "scene/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath> 

namespace lume {

glm::vec3 Camera::forward() const
{
    float y = glm::radians(yaw);
    float p = glm::radians(pitch);
    return {glm::cos(p) * glm::sin(y), glm::sin(p), -glm::cos(p) * glm::cos(y)};
}

glm::vec3 Camera::right() const
{
    return glm::normalize(glm::cross(forward(), glm::vec3(0,1,0)));
}

glm::vec3 Camera::up() const
{
    return glm::cross(right(), forward());
}

glm::mat4 Camera::view() const
{
    const glm::vec3 target = position + forward();
    return glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::proj(float aspect) const
{
    return glm::perspective(glm::radians(fov_y), aspect, near_plane, far_plane);
}

void Camera::lookAt(const glm::vec3& target) {
    const glm::vec3 d = target - position;
    if (glm::dot(d, d) < 1e-8f) return;   // target on top of the camera: keep current angles

    const glm::vec3 f = glm::normalize(d);
    yaw   = glm::degrees(std::atan2(f.x, -f.z));
    pitch = glm::degrees(std::asin(glm::clamp(f.y, -1.0f, 1.0f)));   // clamp: asin domain
    clampPitch();
}

void Camera::clampPitch() {
    pitch = glm::clamp(pitch, -89.0f, 89.0f);
}

}


