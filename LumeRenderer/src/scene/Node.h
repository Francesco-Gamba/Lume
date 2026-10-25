#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cstdint>
#include <glm/gtc/matrix_transform.hpp>
#include <string>


namespace lume {

    constexpr uint32_t NoMesh = UINT32_MAX;
    constexpr uint32_t NoParent = UINT32_MAX;
	
    class Node
	{
        friend class Scene;

        private:
            uint32_t mesh_id_ = NoMesh;
            std::string name_;
            uint32_t parent_ = NoParent;
            glm::vec3 translation_ {0.0f, 0.0f, 0.0f};
            glm::quat rotation_ = glm::identity<glm::quat>();
            glm::vec3 scale_{1.0f, 1.0f, 1.0f};
            glm::mat4 world_matrix_ = glm::identity<glm::mat4>(); 
            bool dirty_ = true;
            void setParent(const uint32_t parent_id);
            void updateWorldMatrix(const glm::mat4& parent_world);

        public:

            glm::mat4 localMatrix() const {
                glm::mat4 t = glm::translate(glm::mat4(1.0f), translation_);
                glm::mat4 r = glm::mat4_cast(rotation_);
                glm::mat4 s = glm::scale(glm::mat4(1.0f), scale_);
                return t*r*s;
            }

            uint32_t getMeshID() const { return mesh_id_; }
            uint32_t getParent() const { return parent_; }
            glm::vec3 getTranslation() const { return translation_; }
            glm::quat getRotation() const { return rotation_; }
            glm::vec3 getScale() const { return scale_; }
            const glm::mat4& worldMatrix() const { return world_matrix_; }
            const std::string& getName() const { return name_; }
            bool isDirty() const { return dirty_; }
            void setMeshID(const uint32_t mesh_id);
            void setTranslation(const glm::vec3& translation);
            void setRotation(const glm::quat& rotation);
            void setScale(const glm::vec3& scale);
            void setName(const std::string& name);
	};

}