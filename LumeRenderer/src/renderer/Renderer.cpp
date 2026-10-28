#include "renderer/Renderer.h"
#include <glad/gl.h>
#include "scene/Scene.h"
#include "core/DebugSettings.h"
#include "scene/SceneLoader.h"


namespace lume {


    void Renderer::render(const Scene& scene,const Camera& camera,const DebugSettings& settings, int width, int height,FrameStats& stats)
    {

        float aspect = float(width) / float(height);

        glViewport(0, 0, width, height);

        glClearColor(settings.clear_color.r, settings.clear_color.g, settings.clear_color.b, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, settings.wireframe ? GL_LINE : GL_FILL);

        // Camera Buffer
        camera_ub_.update(camera, aspect);

        // Shader
        mesh_shader_.bind();
        mesh_shader_.set("uViewMode", (int)settings.view);
        mesh_shader_.set("uSunDir", scene.light.direction);

        stats.draw_calls = 0;

        for (const Node& node : scene.nodes()) {
            if (node.getMeshID() != NoMesh) {
                
                const Mesh& mesh = meshes_[node.getMeshID()];
                Material mat = materials_[mesh.getMaterialIndex()];
                const Texture& tex = (mat.base_color_image == NoTexture) ? white_tex_ : textures_[mat.base_color_image];
                tex.bind(0);

                mesh_shader_.set("uBaseColorFactor", mat.base_color_factor);

                if (settings.cull && !mat.double_sided) {
                    glEnable(GL_CULL_FACE);
                }
                else {
                    glDisable(GL_CULL_FACE);
                }

                mesh_shader_.set("uModel", node.worldMatrix());
                meshes_[node.getMeshID()].draw();
                ++stats.draw_calls;
            }
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

    Renderer::Renderer() : mesh_shader_(LUME_SHADER_DIR "/mesh.glsl")
    {
        glEnable(GL_DEPTH_TEST);
    }

    void Renderer::loadScene(const std::filesystem::path& path, Scene& scene) {
        SceneLoader::load(path, meshes_, scene, textures_, materials_);
    }

    int Renderer::meshCount() const {
        return static_cast<int>(meshes_.size());
    }

    int Renderer::triangleCount() const {
        int count = 0;

        for (const Mesh& mesh : meshes_) {
            count += mesh.indexCount() / 3;
        }

        return count;
    }




}  // namespace lume