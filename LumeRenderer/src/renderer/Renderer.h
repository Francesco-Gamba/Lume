#pragma once 
#include <vector>
#include <filesystem>
#include "gfx/CameraUB.h"
#include "gfx/Shader.h"
#include "gfx/Mesh.h"

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
    };

}  // namespace lume