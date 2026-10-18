#include "gfx/CameraUB.h"


namespace lume {

    void CameraUB::update(const Camera& camera, float aspect)
    {
        camera_data_.proj = camera.proj(aspect);
        camera_data_.view = camera.view();
        

        if (!is_first_frame_) {
            camera_data_.prev_view_proj = camera_data_.view_proj;
            camera_data_.view_proj = camera_data_.proj * camera_data_.view;
        }
        else {
            camera_data_.view_proj = camera_data_.proj * camera_data_.view;
            camera_data_.prev_view_proj = camera_data_.view_proj;
            is_first_frame_ = false;
        }

        camera_data_.position = glm::vec4(camera.position, 1.0);

        glNamedBufferSubData(id_, 0, size_, &camera_data_);
    }

    CameraUB::CameraUB() {
        static_assert(sizeof(CameraData) == 272, "CameraData doesn't match std140 layout");
        static_assert(sizeof(CameraData) % 16 == 0, "UBO size must be a multiple of 16");

        glCreateBuffers(1, &id_);
        glNamedBufferStorage(id_, size_, nullptr, GL_DYNAMIC_STORAGE_BIT);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, id_);
    }

    CameraUB::~CameraUB() {
        glDeleteBuffers(1, &id_);
    }
}


