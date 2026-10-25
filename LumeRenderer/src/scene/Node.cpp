#include "scene/Node.h"

void lume::Node::setMeshID(const uint32_t mesh_id) {
    mesh_id_ = mesh_id;
}

void lume::Node::setParent(const uint32_t parent_id) {
    parent_ = parent_id;
}

void lume::Node::setTranslation(const glm::vec3& translation) {
    translation_ = translation;
    dirty_ = true;
}

void lume::Node::setRotation(const glm::quat& rotation)
{
    rotation_ = rotation;
    dirty_ = true;
}

void lume::Node::setScale(const glm::vec3& scale) {
    scale_ = scale;
    dirty_ = true;
}

void lume::Node::updateWorldMatrix(const glm::mat4& parent_world){
    world_matrix_ = parent_world * localMatrix();
    dirty_ = false;
}

void lume::Node::setName(const std::string& name) {
    name_ = name;
}