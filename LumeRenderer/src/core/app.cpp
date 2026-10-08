#include "core/app.h"
#include "gfx/gl_debug.h"

#include <glm/gtc/matrix_transform.hpp>   // translate, rotate, lookAt, perspective
#include <glm/gtc/type_ptr.hpp>           // value_ptr

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <cfloat>
#include <cstddef>
#include <iostream>
#include <span>


namespace lume {

    struct Vertex
    {
        glm::vec3 pos;
        glm::vec3 normal;
    };

    struct Bbox
    {
        glm::vec3 bmin;
        glm::vec3 bmax;
    };

	App::App() : window(1024, 768, "Lume") {
        std::cout << glGetString(GL_VENDOR) << " | " << glGetString(GL_RENDERER) << " | " << glGetString(GL_VERSION) << "\n";
        install_gl_debug_output();

        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(LUME_ASSET_DIR "/meshes/Combined.glb", aiProcess_Triangulate | aiProcess_GenSmoothNormals| aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices);

        if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) {
            throw std::runtime_error(std::string("assimp: ") + importer.GetErrorString());
        }

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        
        glm::vec3 bmin(FLT_MAX);
        glm::vec3 bmax(-FLT_MAX);


        for (const aiMesh* mesh : std::span(scene->mMeshes, scene->mNumMeshes)) {
            uint32_t base = (uint32_t)vertices.size();
            for (unsigned v = 0; v < mesh->mNumVertices; v++) {
                const aiVector3D& p = mesh->mVertices[v];
                const aiVector3D& n = mesh->mNormals[v];

                glm::vec3 pos(p.x, p.y, p.z);

                vertices.push_back({pos, glm::vec3(n.x, n.y, n.z)});

                bmin = glm::min(bmin, pos);
                bmax = glm::max(bmax, pos);
            }

            for (const aiFace& face : std::span(mesh->mFaces, mesh->mNumFaces)) {
                for (unsigned idx : std::span(face.mIndices, face.mNumIndices)) {
                    indices.push_back(base + idx);
                }
            }
        }

        std::cout << "Loaded " << scene->mNumMeshes << " meshes, " << vertices.size() << " vertices, "
                  << indices.size() / 3 << "triagnles \n"; 


        unsigned int vbo, vao, ebo;

        glCreateBuffers(1, &vbo);
        glNamedBufferStorage(vbo, vertices.size() * sizeof(Vertex), vertices.data(), 0);

        glCreateBuffers(1, &ebo);
        glNamedBufferStorage(ebo, indices.size() * sizeof(uint32_t), indices.data(), 0);

        //describe how to read the buffers
        glCreateVertexArrays(1, &vao);
        
        //attache vbo to slot 0, start at byte 0, one vertex per step
        glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
        
        //attach ebo and vao. each vao has only one element buffer;
        glVertexArrayElementBuffer(vao, ebo);

        //Attribute 0 = pos 3 floats per pos
        glEnableVertexArrayAttrib(vao, 0);
        glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
        glVertexArrayAttribBinding(vao, 0, 0);

        //Attribute 1  = normals, 3 floats per normal, offset of normal, read from slot 0
        glEnableVertexArrayAttrib(vao, 1);
        glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
        glVertexArrayAttribBinding(vao, 1, 0);

        index_count = (GLsizei)indices.size();


	}

	void App::frame() {
        int width, height;
        window.framebuffer_size(width, height);
        glViewport(0, 0, width, height);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void App::run() {
        while (!window.should_close()) {
            frame();
            window.swap_buffers();
            window.poll_events();
        }
	}
}
