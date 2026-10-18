#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "math/BBox.h"
#include "scene/Node.h"
#include <vector>



namespace lume {

	class Scene
	{

		public:
			struct Light	//TO DO: MOVE LATER TO OWN CLASS
			{
                glm::vec3 color {1.0f, 1.0f, 1.0f};
                glm::vec3 direction {1.0f, 1.0f, 1.0f};
			};

			Light light;

			const Node& node(uint32_t index) const;
            Node& node(uint32_t index);

            const std::vector<Node>& nodes() const { return nodes_; }

			uint32_t addNode(const Node& node);
            void removeNode(const uint32_t index);
            uint32_t nodeCount() const { return static_cast<uint32_t>(nodes_.size()); }

            const BBox& getSceneBounds() const { return scene_bounds_; }

			void recomputeSceneBounds(const std::vector<BBox>& mesh_bounds);

		private:
			BBox scene_bounds_;
            std::vector<Node> nodes_;
	};

}