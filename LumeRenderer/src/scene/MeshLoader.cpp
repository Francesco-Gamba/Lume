#include "scene/MeshLoader.h"

#include <stb_image.h>

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <iostream>
#include <span>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

namespace lume {

namespace {  // file-local: Assimp types never leave this .cpp

MeshData convertMesh(const aiMesh* mesh)
{
    MeshData data;
    data.vertices.reserve(mesh->mNumVertices);
    data.material_index = mesh->mMaterialIndex;

    for (unsigned int v = 0; v < mesh->mNumVertices; ++v) {
        Vertex vert{};
        vert.pos = glm::vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
        vert.normal = mesh->HasNormals()
                          ? glm::vec3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z)
                          : glm::vec3(0.0f, 1.0f, 0.0f);

        vert.texcoord = mesh->HasTextureCoords(0)
                            ? glm::vec2(mesh->mTextureCoords[0][v].x,
                                        -mesh->mTextureCoords[0][v].y)  // flipped vertical coordinates
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
    const uint32_t index =
        static_cast<uint32_t>(out.nodes.size() - 1);  // position of the node just pushed = its index

    for (unsigned int c = 0; c < ai_node->mNumChildren; c++) {
        loadNode(ai_node->mChildren[c], index, out);
    }
}

uint32_t loadImage(const aiScene* scene,
                   const std::string& ref,
                   std::unordered_map<std::string, uint32_t>& cache,
                   const std::filesystem::path& model_dir,
                   ImportedFile& out)
{
    auto it = cache.find(ref);
    if (it != cache.end()) {
        return it->second;
    }

    const aiTexture* texture = scene->GetEmbeddedTexture(ref.c_str());
    int w = 0, h = 0, comp = 0;
    stbi_uc* data = nullptr;

    if (texture != nullptr) {
        if (texture->mHeight != 0) {
            std::cerr << "loadImage: " << ref << ": uncompressed embedded texture (" << texture->mWidth << "x"
                      << texture->mHeight << ") not supported\n";
            cache[ref] = NoImage;
            return NoImage;
        }

        data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(texture->pcData),
                                     static_cast<int>(texture->mWidth),
                                     &w,
                                     &h,
                                     &comp,
                                     4);  // force RGBA

        if (!data) {
            std::cerr << "stb: " << ref << ": " << stbi_failure_reason() << "\n";
            cache[ref] = NoImage;
            return NoImage;
        }
    }
    else {
        const std::filesystem::path full = model_dir / ref;
        data = stbi_load(full.string().c_str(), &w, &h, &comp, 4);
        if (!data) {
            std::cerr << "stb: " << full.string() << ": " << stbi_failure_reason() << "\n";
            cache[ref] = NoImage;
            return NoImage;
        }
    }

    ImportedImage imported_image;
    imported_image.height = h;
    imported_image.width = w;
    imported_image.pixels.assign(data, data + size_t(w) * h * 4);

    stbi_image_free(data);
    out.images.push_back(std::move(imported_image));

    const uint32_t index = static_cast<uint32_t>(out.images.size() - 1);

    std::cout << "Loaded image with index: [" << index << "] \n"
              << "  Width: " << w << "\n    Height: " << h << "\n    Comp: " << comp << "\n";

    cache[ref] = index;
    return index;
}

}  // namespace

ImportedFile importFile(const std::filesystem::path& path)
{
    Assimp::Importer importer;
    ImportedFile out;

    const aiScene* scene = importer.ReadFile(
        path.string(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
    if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) {
        throw std::runtime_error(std::string("assimp: ") + importer.GetErrorString());
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        out.meshes.push_back(convertMesh(scene->mMeshes[i]));
    }

    std::unordered_map<std::string, uint32_t> cache;

    for (unsigned int m = 0; m < scene->mNumMaterials; m++) {
        ImportedMaterial mat;
        const aiMaterial* ai_mat = scene->mMaterials[m];

        aiColor4D c;
        if (ai_mat->Get(AI_MATKEY_BASE_COLOR, c) == AI_SUCCESS) {
            mat.base_color_factor = glm::vec4(c.r, c.g, c.b, c.a);
        }

        int two_sided = 0;
        if (ai_mat->Get(AI_MATKEY_TWOSIDED, two_sided) == AI_SUCCESS) {
            mat.double_sided = (two_sided != 0);
        }

        aiString tex;
        if (ai_mat->GetTexture(aiTextureType_BASE_COLOR, 0, &tex) == AI_SUCCESS ||
            ai_mat->GetTexture(aiTextureType_DIFFUSE, 0, &tex) == AI_SUCCESS)
        {
            mat.base_color_image = loadImage(scene, tex.C_Str(), cache, path.parent_path(), out);
        }

        out.materials.push_back(mat);
        std::cout << "Material[" << out.materials.size() - 1 << "] :" << "\n";
        std::cout << "  base color factor: " << mat.base_color_factor.x << " " << mat.base_color_factor.y
                  << " " << mat.base_color_factor.z << " " << mat.base_color_factor.w << "\n";
        std::cout << "  two sided: " << mat.double_sided << "\n";
        std::cout << "  base color texture: " << mat.base_color_image << "\n";
    }

    loadNode(scene->mRootNode, NoParent, out);
    return out;
}

}  // namespace lume