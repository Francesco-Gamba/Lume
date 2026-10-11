#pragma once

#include <glm/glm.hpp>
#include <cfloat>


namespace lume {
class BBox
{
public:
    BBox() = default;
    BBox(glm::vec3 new_min, glm::vec3 new_max) : min(new_min), max(new_max) {}

    glm::vec3 min = glm::vec3(+FLT_MAX);
    glm::vec3 max = glm::vec3(-FLT_MAX);

    void expand(const glm::vec3& point)
    {
        min = glm::min(min, point);
        max = glm::max(max, point);
    }

    void expand(const BBox& box)
    {
        min = glm::min(min, box.min);
        max = glm::max(max, box.max);
    }

    glm::vec3 center() const { return (min + max) * 0.5f; }

    glm::vec3 extent() const { return max - min; }

    float radius() const { return glm::length(max - min) * 0.5f; }

    bool isValid() const { return glm::all(glm::lessThanEqual(min, max)); }
};
}  // namespace lume