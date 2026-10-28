#pragma once 
#include <vector>
#include <filesystem>
#include "gfx/CameraUB.h"
#include "gfx/Shader.h"
#include "gfx/Mesh.h"
#include "gfx/Texture.h"
#include "gfx/Material.h"

namespace lume {


    class Camera;
    class Scene;
    struct FrameStats;
    struct DebugSettings;


    class Renderer
    {
        public:
            Renderer();

            void loadScene(const std::filesystem::path& path, Scene& scene);
            int meshCount() const;
            int triangleCount() const;
            void render(const Scene& scene,const Camera& camera,const DebugSettings& settings,int width,int height,FrameStats& stats);

        private:
            Shader mesh_shader_;
            CameraUB camera_ub_;
            std::vector<Mesh> meshes_;
            std::vector<Texture> textures_;
            std::vector<Material> materials_;
            Texture white_tex_{1, 1, false, {255, 255, 255, 255}};
            
    };

}  // namespace lume