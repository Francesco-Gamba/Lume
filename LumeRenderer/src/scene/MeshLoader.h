#pragma once
#include "gfx/Mesh.h"
#include <filesystem>
#include <vector>

namespace lume {

	// Loads every mesh in a model file as CPU-side data, one MeshData per mesh.
	// Throws std::runtime_error if the file can't be read.
	std::vector<MeshData> loadMesh(const std::filesystem::path& path);

}  // namespace lume