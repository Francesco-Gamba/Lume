#include "scene/MeshLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>
#include <span>
#include <string>
#include <utility>

namespace lume {
	
	std::vector<MeshData> loadMesh(const std::filesystem::path& path) {
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(path.string(),aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices);

        if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) {
            throw std::runtime_error(std::string("assimp: ") + importer.GetErrorString());
        }

        std::vector<MeshData> result;
        result.reserve(scene->mNumMeshes);

        for (const aiMesh* mesh : std::span(scene->mMeshes, scene->mNumMeshes)) {
            // Reserve exact sizes up front so the vectors never reallocate while filling
            MeshData data;
            data.vertices.reserve(mesh->mNumVertices);
            data.indices.reserve(mesh->mNumFaces * 3);

            for (unsigned v = 0; v < mesh->mNumVertices; v++) {
                const aiVector3D& p = mesh->mVertices[v];
                const aiVector3D& n = mesh->mNormals[v];

                glm::vec3 pos(p.x, p.y, p.z);
                glm::vec3 normal(n.x, n.y, n.z);

                data.vertices.push_back({ pos, normal });
                data.bounds.expand(pos);
            }

            for (const aiFace& face : std::span(mesh->mFaces, mesh->mNumFaces)) {
                for (unsigned idx : std::span(face.mIndices, face.mNumIndices)) {
                    data.indices.push_back(idx);
                }
            }

            result.push_back(std::move(data));   // hand over the vectors, no copy
        }

        return result;
	}

}
