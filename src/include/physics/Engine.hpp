#pragma once

#include <physics/Entity.hpp>
#include <physics/collisions.hpp>
#include <physics/general.hpp>
#include <physics/Manifold.hpp>

#include <iostream>
#include <vector>
#include <memory>

namespace Physics
{
    class Engine
    {
    public:
        void addEntity(std::shared_ptr<Entity> entity)
        {
            m_entities.push_back(entity);
        }

        void update(float dt, size_t iterations)
        {
            for (auto &entity : m_entities)
                entity->applyForceToCenter(m_gravity);

            for (int i = 0; i < iterations; i++)
                step(dt / (float)iterations);

            for (auto &entity : m_entities)
                entity->clearForces();
        }

        void render()
        {
            for (auto &entity : m_entities)
                entity->render();
        }

        void setGravity(const glm::vec2 &gravity) { m_gravity = gravity; }

    private:
        void step(float dt);

        glm::vec2 m_gravity;
        std::vector<std::shared_ptr<Entity>> m_entities;
    };
}; // namespace Physics
