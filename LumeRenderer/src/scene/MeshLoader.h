#pragma once
#include "gfx/Mesh.h"
#include <filesystem>
#include <vector>
#include "scene/Node.h"


namespace lume {

	struct ImportedNode
    {
        std::string name;
        uint32_t parent = NoParent;  // index into ImportedFile::nodes
        glm::vec3 translation{0.0f};
        glm::quat rotation = glm::identity<glm::quat>();
        glm::vec3 scale{1.0f};
        std::vector<uint32_t> meshes;  // indices into ImportedFile::meshes (can be 0, 1 or many)
    };

    struct ImportedFile
    {
        std::vector<MeshData> meshes;     // meshes[i] == Assimp mesh i
        std::vector<ImportedNode> nodes;  // parents always before children
    };

    ImportedFile importFile(const std::filesystem::path& path);


}  // namespace lume