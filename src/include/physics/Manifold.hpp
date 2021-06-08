#pragma once

#include <memory>
#include <vector>

#include <physics/definitions.hpp>

namespace Physics
{
    struct Manifold
    {
        Entity *a;
        Entity *b;

        bool isColliding;
        float penetration;
        glm::vec2 normal;

        std::vector<glm::vec2> contact_points;
    };
} // namespace Physics
