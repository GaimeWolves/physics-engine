#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <physics/Entity.hpp>
#include <defs.hpp>
#include <physics/collisions.hpp>
#include <physics/Manifold.hpp>

namespace Physics
{
    class Edge final : public Entity
    {
    public:
        Edge(glm::vec2 a, glm::vec2 b);

        void render() override;

        glm::vec2 getNormal() { return m_edge_normal; }

        glm::vec2 getVertexA() const { return m_point_a; }
        glm::vec2 getVertexB() const { return m_point_b; }

        void setMass(float mass) override
        {
            m_mass = 0;
            m_inv_mass = 0;
            m_inertia = 0;
            m_inv_inertia = 0;
        }

    private:
        void updateBoundingBox() override;

        glm::vec2 m_point_a, m_point_b;
        glm::vec2 m_edge_normal;
    };
} // namespace Physics