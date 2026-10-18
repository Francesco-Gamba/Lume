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

    uint32_t Scene::addNode(const Node& node)
    {
        nodes_.push_back(node);
        return static_cast<uint32_t>(nodes_.size()) - 1;
    }

    void Scene::removeNode(const uint32_t index)
    {
        assert(index < nodes_.size());
        nodes_[index] = Node();
    }

    void Scene::recomputeSceneBounds(const std::vector<BBox>& mesh_bounds)
    {
        BBox new_bounds = BBox();

        for (const Node& node : nodes_) {
            if (node.mesh_id == NoMesh) {
                continue;
            }

            assert(node.mesh_id < mesh_bounds.size());

            const BBox& local = mesh_bounds[node.mesh_id];
            const glm::mat4 M = node.localMatrix();

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