#pragma once
#include "math/BBox.h"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <vector>
#include <cstdint>




namespace lume {

	struct Vertex {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 texcoord;
    };

    struct MeshData
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        BBox bounds;
        uint32_t material_index;
    };

    class Mesh
    {
        public:

            explicit Mesh(const MeshData& data);
            ~Mesh();


            Mesh(const Mesh&) = delete;
            Mesh& operator = (const Mesh&) = delete;

            Mesh(Mesh&& other) noexcept;
            Mesh& operator = (Mesh&& other) noexcept; 

            void draw() const;

            const BBox& bounds() const { return bounds_; }
            GLsizei indexCount() const { return index_count_; }
            uint32_t getMaterialIndex() const { return material_index_; }


        private:
            void release();
    
            GLuint vao_ = 0;
            GLuint vbo_ = 0;
            GLuint ebo_ = 0;

            GLsizei index_count_ = 0;
            BBox bounds_;
            uint32_t material_index_;

    };

}