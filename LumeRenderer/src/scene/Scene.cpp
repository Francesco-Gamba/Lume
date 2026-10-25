#include "scene/Scene.h"
#include <cassert>

namespace lume {
    const Node& Scene::node(uint32_t index) const
    {
        assert(index < nodes_.size());
        return nodes_[index];
    }

    Node& Scene::node(uint32_t index)
    {
        assert(index < nodes_.size());
        return nodes_[index];
    }

    uint32_t Scene::addNode(const Node& node, uint32_t parent)
    {
        assert(parent == NoParent || parent < nodes_.size());   // a parent must already exist

        nodes_.push_back(node);
        nodes_.back().setParent(parent);                        // allowed: Scene is Node's friend
        return static_cast<uint32_t>(nodes_.size() - 1);
    }

    void Scene::updateTransforms()
    {
        static const glm::mat4 kIdentity(1.0f);
        std::vector<bool> updated(nodes_.size(), false);        // recomputed in THIS pass

        for (size_t i = 0; i < nodes_.size(); ++i) {
            Node& n = nodes_[i];
            const bool has_parent     = n.parent_ != NoParent;
            const bool parent_updated = has_parent && updated[n.parent_];

            if (n.dirty_ || parent_updated) {
                const glm::mat4& parent_world = has_parent ? nodes_[n.parent_].world_matrix_ : kIdentity;
                n.updateWorldMatrix(parent_world);
                updated[i] = true;
            }
        }
    }

    void Scene::removeNode(const uint32_t index)
    {
        assert(index < nodes_.size());
        nodes_[index] = Node();
    }

    void Scene::recomputeBounds(const std::vector<BBox>& mesh_bounds)
    {
        BBox new_bounds = BBox();

        for (const Node& node : nodes_) {
            if (node.getMeshID() == NoMesh) {
                continue;
            }

            assert(node.getMeshID() < mesh_bounds.size());

            const BBox& local = mesh_bounds[node.getMeshID()];
            const glm::mat4& M = node.worldMatrix();

            for (float x : {local.min.x, local.max.x}) {
                for (float y : {local.min.y, local.max.y}) {
                    for (float z : {local.min.z, local.max.z}) {
                        const glm::vec4 world = M * glm::vec4(x, y, z, 1.0f);
                        new_bounds.expand(glm::vec3(world));
                    }
                }
            }
        }

        scene_bounds_ = new_bounds;
    }


    }  // namespace lume