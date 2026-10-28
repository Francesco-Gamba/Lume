#pragma once
#include <vector>
#include <filesystem>
#include "scene/MeshLoader.h"


namespace lume {
	class Scene; 
	class Mesh;
    class Texture;
    struct Material;
    }  // namespace lume

namespace lume::SceneLoader {
	void load(const std::filesystem::path& path, std::vector<Mesh>& meshes, Scene& scene, std::vector<Texture>& textures, std::vector<Material>& materials);
}