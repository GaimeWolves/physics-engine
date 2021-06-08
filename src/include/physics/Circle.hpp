#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <physics/Entity.hpp>
#include <defs.hpp>
#include <physics/collisions.hpp>
#include <physics/Manifold.hpp>

namespace Physics
{
    class Circle final : public Entity
    {
    public:
        Circle(glm::vec2 position, float radius);

        void render() override;

        float getRadius() const { return m_radius; }

        void setMass(float mass) override
        {
            m_mass = mass;

            if (m_mass == PHYSICS_INF_MASS)
            {
                m_inv_mass = 0;
                m_inertia = 0;
                m_inv_inertia = 0;
                return;
            }

            m_inv_mass = 1 / m_mass;
            m_inertia = m_mass * m_radius * m_radius;
            m_inv_inertia = 1 / m_inertia;
        }

    private:
        void updateBoundingBox() override;

        float m_radius;
    };
} // namespace Physics