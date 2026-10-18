#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include "scene/Camera.h"

namespace lume {


	class CameraUB
	{
		public:
			CameraUB();
            
            CameraUB(const CameraUB&) = delete;
            CameraUB& operator=(const CameraUB&) = delete;

            CameraUB(CameraUB&& other) = delete;
            CameraUB& operator =(CameraUB&& other) = delete; 
            
            ~CameraUB();
            void update(const Camera& camera, float aspect);

            size_t size() const { return size_;}
			GLuint id() const { return id_; }

            void resetHistory() { is_first_frame_ = true;}

		private:
            struct CameraData       //Must match uniform camera block in Mesh.glsl or other shaders in the future
            {
                glm::mat4 view;
                glm::mat4 proj;
                glm::mat4 view_proj;
                glm::mat4 prev_view_proj;
                glm::vec4 position;
            };

            GLuint id_ = 0;
            size_t size_ = sizeof(CameraData);
            CameraData camera_data_ {};
            bool is_first_frame_ = true;
	};

}
