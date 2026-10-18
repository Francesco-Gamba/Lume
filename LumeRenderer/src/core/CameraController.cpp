#include "core/CameraController.h"

#include <cmath>

namespace lume {

glm::vec3 CameraController::pivot(const Camera& camera) const
{
    return camera.position + camera.forward() * pivot_distance;
}

void CameraController::lookAround(Camera& camera, glm::vec2 delta) const
{
    camera.yaw += delta.x * look_sensitivity;
    camera.pitch -= delta.y * look_sensitivity;  
    camera.clampPitch();
}

void CameraController::fly(Camera& camera, const InputState& in, float dt)
{
    const float forw = static_cast<float>(in.down(Key::W)) - static_cast<float>(in.down(Key::S));
    const float right = static_cast<float>(in.down(Key::D)) - static_cast<float>(in.down(Key::A));
    const float vert = static_cast<float>(in.down(Key::E)) - static_cast<float>(in.down(Key::Q));

    const glm::vec3 dir = forw * camera.forward() + right * camera.right() + vert * camera.up();

    if (glm::dot(dir, dir) > 0.0f) {
        const float boost = in.down(Key::LeftShift) ? boost_multiplier : 1.0f;
        camera.position += glm::normalize(dir) * fly_speed * boost * dt;
    }

    if (in.scroll != 0.0f) {
        fly_speed = glm::clamp(fly_speed * std::pow(1.2f, in.scroll), 0.01f, 1000.0f);
    }
}

void CameraController::pan(Camera& camera, glm::vec2 delta, float viewport_height) const
{
    const float world_per_pixel =
        2.0f * pivot_distance * std::tan(glm::radians(camera.fov_y) * 0.5f) / viewport_height;

    camera.position += (-camera.right() * delta.x + camera.up() * delta.y) * world_per_pixel;
}

void CameraController::orbit(Camera& camera, glm::vec2 delta) const
{
    const glm::vec3 p = pivot(camera); 

    camera.yaw += delta.x * rotation_sensitivity;
    camera.pitch -= delta.y * rotation_sensitivity;
    camera.clampPitch();

    camera.position = p - camera.forward() * pivot_distance;
}

void CameraController::zoomBy(Camera& camera, float factor)
{
    const glm::vec3 p = pivot(camera);
    pivot_distance = glm::max(pivot_distance * factor, min_distance);
    camera.position = p - camera.forward() * pivot_distance;
}

void CameraController::cameraUpdate(const InputState& in, Camera& camera, float dt, float viewport_height)
{
    const bool alt = in.down(Key::LeftAlt);

    if (drag_ == Drag::None) {
        if (alt && in.pressed(MouseButton::Left)) {
            drag_ = Drag::Orbit;
            drag_button_ = MouseButton::Left;
        }
        else if (alt && in.pressed(MouseButton::Right)) {
            drag_ = Drag::Zoom;
            drag_button_ = MouseButton::Right;
        }
        else if (in.pressed(MouseButton::Right)) {
            drag_ = Drag::Look;
            drag_button_ = MouseButton::Right;
        }
        else if (in.pressed(MouseButton::Middle)) {
            drag_ = Drag::Pan;
            drag_button_ = MouseButton::Middle;
        }
    }

    if (drag_ != Drag::None && !in.down(drag_button_)) {
        drag_ = Drag::None;
    }

    switch (drag_) {
    case Drag::Look:
        lookAround(camera, in.mouse_delta);
        fly(camera, in, dt); 
        break;

    case Drag::Orbit: orbit(camera, in.mouse_delta); break;

    case Drag::Pan: pan(camera, in.mouse_delta, viewport_height); break;

    case Drag::Zoom:
        zoomBy(camera, std::exp(in.mouse_delta.y * drag_zoom_sensitivity));
        break;

    case Drag::None:
        if (in.scroll != 0.0f) {
            zoomBy(camera, std::pow(1.0f - zoom_step, in.scroll));   // +1 notch: 10% closer, -1: back out
        }
        break;
    }
}

}  // namespace lume