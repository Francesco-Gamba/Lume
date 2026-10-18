#pragma once
#include "core/Input.h"
#include "scene/Camera.h"

#include <glm/glm.hpp>

namespace lume {

    class CameraController
    {
        public:
            float look_sensitivity = 0.15f; 
            float rotation_sensitivity = 0.5f;    
            float fly_speed = 10.0f;             
            float boost_multiplier = 2.0f;        
            float zoom_step = 0.1f;
            float drag_zoom_sensitivity = 0.01f;  
            float min_distance = 0.05f;          
            float pivot_distance = 10.0f;         

            void cameraUpdate(const InputState& in, Camera& camera, float dt, float viewport_height);

        private:
            enum class Drag { None, Look, Orbit, Pan, Zoom };

            Drag drag_ = Drag::None;
            MouseButton drag_button_ = MouseButton::Left; 

            glm::vec3 pivot(const Camera& camera) const;
            void lookAround(Camera& camera, glm::vec2 delta) const;
            void fly(Camera& camera, const InputState& in, float dt);
            void pan(Camera& camera, glm::vec2 delta, float viewport_height) const;
            void orbit(Camera& camera, glm::vec2 delta) const;
            void zoomBy(Camera& camera, float factor);
    };

}  // namespace lume