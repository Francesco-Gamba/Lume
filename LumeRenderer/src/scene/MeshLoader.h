#pragma once
#include "gfx/Mesh.h"
#include <filesystem>
#include <cstdint>
#include <vector>
#include "scene/Node.h"
#include <string>

namespace lume {

    constexpr uint32_t NoImage = UINT32_MAX;

	struct ImportedNode
    {
        std::string name;
        uint32_t parent = NoParent;  // index into ImportedFile::nodes
        glm::vec3 translation{0.0f};
        glm::quat rotation = glm::identity<glm::quat>();
        glm::vec3 scale{1.0f};
        std::vector<uint32_t> meshes;  // indices into ImportedFile::meshes (can be 0, 1 or many)
    };

    struct ImportedMaterial
    {
        glm::vec4 base_color_factor{1, 1, 1, 1};
        uint32_t base_color_image = NoImage;
        bool double_sided = false;
    };

    struct ImportedImage
    {
        int width = 0;
        int height = 0;
        std::vector<uint8_t> pixels;
    };

    struct ImportedFile
    {
        std::vector<MeshData> meshes;     // meshes[i] == Assimp mesh i
        std::vector<ImportedNode> nodes;  // parents always before children
        std::vector<ImportedMaterial> materials;
        std::vector<ImportedImage> images;
    };

    ImportedFile importFile(const std::filesystem::path& path);


}  // namespace lume