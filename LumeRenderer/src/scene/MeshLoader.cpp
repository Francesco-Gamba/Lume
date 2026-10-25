#include "scene/MeshLoader.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <stdexcept>
#include <span>
#include <string>
#include <utility>

namespace lume {

  namespace {   // file-local: Assimp types never leave this .cpp

    MeshData convertMesh(const aiMesh* mesh)
    {
        MeshData data;
        data.vertices.reserve(mesh->mNumVertices);

        for (unsigned int v = 0; v < mesh->mNumVertices; ++v) {
            Vertex vert{};
            vert.pos = glm::vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
            vert.normal = mesh->HasNormals()
                            ? glm::vec3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z)
                            : glm::vec3(0.0f, 1.0f, 0.0f);

            vert.texcoord = mesh->HasTextureCoords(0)
                                ? glm::vec2(mesh->mTextureCoords[0][v].x, -mesh->mTextureCoords[0][v].y)    //flipped vertical coordinates
                                : glm::vec2(0, 0);

            data.bounds.expand(vert.pos);
            data.vertices.push_back(vert);
        }

        data.indices.reserve(static_cast<size_t>(mesh->mNumFaces) * 3);
        for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
            const aiFace& face = mesh->mFaces[f];
            for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                data.indices.push_back(face.mIndices[k]);
            }
        }

        return data;
    }

    void loadNode(const aiNode* ai_node, uint32_t parent, ImportedFile& out)
    {
        ImportedNode node;
        aiVector3D s, t;
        aiQuaternion r;

        node.name = ai_node->mName.C_Str();
        node.parent = parent;

        ai_node->mTransformation.Decompose(s, r, t);
        node.translation = glm::vec3(t.x, t.y, t.z);
        node.rotation = glm::quat(r.w, r.x, r.y, r.z);
        node.scale = glm::vec3(s.x, s.y, s.z);

        for (unsigned int k = 0; k < ai_node->mNumMeshes; k++) {
            node.meshes.push_back(ai_node->mMeshes[k]);
        }

        out.nodes.push_back(std::move(node));
        const uint32_t index = static_cast<uint32_t>(out.nodes.size() - 1);  // position of the node just pushed = its index

        for (unsigned int c = 0; c < ai_node->mNumChildren; c++) {
            loadNode(ai_node->mChildren[c], index, out);
        }
    }

  }  // namespace

    ImportedFile importFile (const std::filesystem::path& path)
    {
        Assimp::Importer importer;
        ImportedFile out;

        const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
        if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) {
            throw std::runtime_error(std::string("assimp: ") + importer.GetErrorString());
        }
        
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            out.meshes.push_back(convertMesh(scene->mMeshes[i]));
        }
        
        loadNode(scene->mRootNode, NoParent, out);
        return out;
    }

   
}