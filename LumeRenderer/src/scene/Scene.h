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
			struct Light	//BETTER TO MOVE LATER TO OWN CLASS
			{
                glm::vec3 color;
                glm::vec3 direction;
			};

			void addNode(const Node& node);
            void removeNode(const uint32_t index);
            Node& getNodeByIndex(const uint32_t index);
            const BBox& getSceneBounds() const { return scene_bounds_; }

			private:
				BBox scene_bounds_;
                std::vector<Node> nodes_;

	};


}