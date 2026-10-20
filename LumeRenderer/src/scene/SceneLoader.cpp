#include "scene/SceneLoader.h"
#include "gfx/Mesh.h"
#include "scene/MeshLoader.h"
#include "scene/Scene.h"
#include <iostream>

namespace lume::SceneLoader {
    void load(const std::filesystem::path& path, std::vector<Mesh>& meshes, Scene& scene)
    {
        std::vector<MeshData> mesh_data = loadMesh(path);
        std::vector<BBox> meshes_bounds;
        meshes_bounds.reserve(mesh_data.size());

        for (int i = 0; i < static_cast<int>(mesh_data.size()); i++) {
            meshes.emplace_back(mesh_data[i]);
            meshes_bounds.push_back(mesh_data[i].bounds);

            Node node;
            node.mesh_id = static_cast<uint32_t>(i);
            scene.addNode(node);
        }

        scene.recomputeBounds(meshes_bounds);

        std::cout << meshes.size() << " Mesh(es) loaded in scene: " << scene.getName() << " \n";
    }
}