#include "gfx/Mesh.h"
#include <glad/gl.h>
#include <cstddef>
#include <utility>

namespace lume {

    Mesh::Mesh(const MeshData& data)
    {
        index_count_ = (GLsizei)data.indices.size(); 
        bounds_ = data.bounds;
        material_index_ = data.material_index;

        glCreateBuffers(1, &vbo_);
        glNamedBufferStorage(vbo_, data.vertices.size() * sizeof(Vertex), data.vertices.data(), 0);

        glCreateBuffers(1, &ebo_);
        glNamedBufferStorage(ebo_, data.indices.size() * sizeof(uint32_t), data.indices.data(), 0);

        glCreateVertexArrays(1, &vao_);

        glVertexArrayVertexBuffer(vao_, 0, vbo_, 0, sizeof(Vertex));

        glVertexArrayElementBuffer(vao_, ebo_);

        glEnableVertexArrayAttrib(vao_, 0);
        glVertexArrayAttribFormat(vao_, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
        glVertexArrayAttribBinding(vao_, 0, 0);

        glEnableVertexArrayAttrib(vao_, 1);
        glVertexArrayAttribFormat(vao_, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
        glVertexArrayAttribBinding(vao_, 1, 0);

        glEnableVertexArrayAttrib(vao_, 2);
        glVertexArrayAttribFormat(vao_, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texcoord));
        glVertexArrayAttribBinding(vao_, 2, 0);
    }

    Mesh::~Mesh()
    {
        release();
    }

    Mesh::Mesh(Mesh&& other) noexcept
    {
        vao_ = other.vao_;
        vbo_ = other.vbo_;
        ebo_ = other.ebo_;
        index_count_ = other.index_count_;
        bounds_ = other.bounds_;
        material_index_ = other.material_index_;

        other.vao_ = 0;
        other.ebo_ = 0;
        other.vbo_ = 0;
        other.index_count_ = 0;
    }

    Mesh& Mesh::operator=(Mesh&& other) noexcept 
    {
        if (this != &other) {
            release();

            vao_ = std::exchange(other.vao_, 0);
            vbo_ = std::exchange(other.vbo_, 0); 
            ebo_ = std::exchange(other.ebo_, 0); 
            index_count_ = std::exchange(other.index_count_, 0);
            bounds_ = other.bounds_;
            material_index_ = other.material_index_;
        }

        return *this;
    }



    void Mesh::draw() const
    {
        glBindVertexArray(vao_);
        glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, nullptr);
    }

    void Mesh::release()
    {
        if (vao_ != 0) {
            glDeleteVertexArrays(1, &vao_);
            vao_ = 0;
        }

        if (vbo_ != 0) {
            glDeleteBuffers(1, &vbo_);
            vbo_ = 0;
        }

        if (ebo_ != 0) {
            glDeleteBuffers(1, &ebo_);
            ebo_ = 0;
        }

        index_count_ = 0;
    }
}


	
