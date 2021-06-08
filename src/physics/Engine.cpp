#include <physics/Engine.hpp>

namespace Physics
{
    void Engine::step(float dt)
    {
        // 1. Integration phase
        for (auto &entity : m_entities)
            entity->update(dt);

        // 2. Collision phase
        std::vector<Manifold> collisions;

        for (auto &a : m_entities)
        {
            for (auto &b : m_entities)
            {
                if (a != b)
                {
                    // 2.a Broad phase (AABB check)
                    if (mayCollide(*a, *b))
                    {
                        // 2.b Narrow phase (Exact check)
                        auto manifold = a->collidesWith(*b);

                        if (manifold.isColliding)
                        {
                            collisions.push_back(manifold);
                        }
                    }
                }
            }
        }

        // 3. Resolving phase
        // 3.a Resolve impulses
        float gravityStrength = glm::length(dt * m_gravity);

        for (auto &manifold : collisions)
            resolveCollision(manifold, gravityStrength);

        // 3.b Correct positions
        for (auto &manifold : collisions)
            positionalCorrection(manifold);
    }
} // namespace Physics