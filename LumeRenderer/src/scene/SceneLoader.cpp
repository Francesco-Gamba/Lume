#include "scene/SceneLoader.h"
#include "gfx/Mesh.h"
#include "scene/MeshLoader.h"
#include "scene/Scene.h"
#include <iostream>

namespace lume::SceneLoader {

    void load(const std::filesystem::path& path, std::vector<Mesh>& meshes, Scene& scene)
    {
        ImportedFile file = importFile(path);

        std::vector<BBox> mesh_bounds;
        mesh_bounds.reserve(file.meshes.size());

        for (const MeshData& data : file.meshes) {
            meshes.emplace_back(data);
            mesh_bounds.push_back(data.bounds);
        }

        std::vector<uint32_t> remap(file.nodes.size(), NoParent);

        for (size_t i = 0; i < file.nodes.size(); ++i) {
            const ImportedNode& rec = file.nodes[i];
            const uint32_t parent = (rec.parent == NoParent) ? NoParent : remap[rec.parent];

            Node node;
            node.setName(rec.name);
            node.setTranslation(rec.translation);
            node.setRotation(rec.rotation);
            node.setScale(rec.scale);

            if (!rec.meshes.empty()) {
                node.setMeshID(rec.meshes[0]);
            }

            remap[i] = scene.addNode(node, parent);

            for (size_t k = 1; k < rec.meshes.size(); ++k) {
                Node extra;
                extra.setName(rec.name + "_" + std::to_string(k));
                extra.setMeshID(rec.meshes[k]);
                scene.addNode(extra, remap[i]);
            }
        }

        scene.updateTransforms();
        scene.recomputeBounds(mesh_bounds);

        std::cout << meshes.size() << " Mesh(es) loaded in scene: " << scene.getName() << " \n";
    }


}

