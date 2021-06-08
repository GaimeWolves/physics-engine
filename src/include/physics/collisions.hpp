#pragma once

#include <physics/definitions.hpp>

namespace Physics
{
    void positionalCorrection(Manifold &m);

    bool mayCollide(const Entity &a, const Entity &b);
    Manifold isColliding(Entity &eA, Entity &eB);

    Manifold checkCircleCircle(Entity &eA, Entity &eB);
    Manifold checkCircleEdge(Entity &eA, Entity &eB);

    Manifold checkEdgeCircle(Entity &eA, Entity &eB);
    Manifold checkEdgeEdge(Entity &eA, Entity &eB);

    void resolveCollision(Manifold &m, float gravityStrength);
} // namespace Physics