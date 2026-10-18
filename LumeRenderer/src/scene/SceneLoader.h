#pragma once
#include <vector>
#include <filesystem>

namespace lume {
	class Mesh;
	class Scene;
	void loadScene(const std::filesystem::path& path, std::vector<Mesh>& meshes, Scene& scene);
}