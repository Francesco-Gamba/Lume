#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "math/BBox.h"
#include "scene/Node.h"
#include <vector>
#include <string>



namespace lume {

	class Scene
	{

		public:
			struct Light	//TO DO: MOVE LATER TO OWN CLASS
			{
                glm::vec3 color {1.0f, 1.0f, 1.0f};
                glm::vec3 direction {0.5f, 1.0f, 0.3f};   // points TOWARD the light, world space (normalized in the shader)
			};

			Light light;

			const Node& node(uint32_t index) const;
            Node& node(uint32_t index);

            const std::vector<Node>& nodes() const { return nodes_; }

			uint32_t addNode(const Node& node);
            void removeNode(const uint32_t index);
            uint32_t nodeCount() const { return static_cast<uint32_t>(nodes_.size()); }

            const BBox& getSceneBounds() const { return scene_bounds_; }

			void recomputeBounds(const std::vector<BBox>& mesh_bounds);

			std::string getName() const { return name_; }
            void setName(std::string name) { name_ = name; }

		private:
            std::string name_ = "New Scene";
			BBox scene_bounds_;
            std::vector<Node> nodes_;
	};

}