#include <physics/collisions.hpp>

#include <iostream>
#include <functional>

#include <limits>

#include <physics/Entity.hpp>
#include <physics/Circle.hpp>
#include <physics/Edge.hpp>
#include <physics/Manifold.hpp>
#include <util.hpp>

namespace Physics
{
    static std::function<Manifold(Entity &, Entity &)> collisionJumpTable[2][2] = {
        {checkCircleCircle, checkCircleEdge},
        {checkEdgeCircle, checkEdgeEdge},
    };

    void positionalCorrection(Manifold &m)
    {
        constexpr float percent = 0.2;
        constexpr float slop = 0.00001;

        auto correction = std::max(m.penetration - slop, 0.0f) / (m.a->getInvMass() + m.b->getInvMass()) * percent * m.normal;
        m.a->move(-m.a->getInvMass() * correction);
        m.b->move(m.b->getInvMass() * correction);
    }

    bool mayCollide(const Entity &a, const Entity &b)
    {
        return a.getBoundingBox().intersects(b.getBoundingBox());
    }

    Manifold isColliding(Entity &eA, Entity &eB)
    {
        return collisionJumpTable[(int)eA.getType()][(int)eB.getType()](eA, eB);
    }

    Manifold checkCircleCircle(Entity &eA, Entity &eB)
    {
        Circle &a = static_cast<Circle &>(eA);
        Circle &b = static_cast<Circle &>(eB);

        Manifold m;
        m.a = &a;
        m.b = &b;

        auto n = b.getPosition() - a.getPosition();
        float r = a.getRadius() + b.getRadius();

        m.isColliding = (n.x * n.x + n.y * n.y) < r * r;

        if (!m.isColliding)
            return m;

        float d = glm::length(n);

        if (d != 0)
        {
            m.penetration = r - d;
            m.normal = n / d;
            m.contact_points.push_back(a.getPosition() + m.normal * a.getRadius());
        }
        else
        {
            m.penetration = a.getRadius();
            m.normal = glm::vec2(1, 0);
            m.contact_points.push_back(a.getPosition());
        }

        return m;
    }

    Manifold checkCircleEdge(Entity &eA, Entity &eB)
    {
        auto m = checkEdgeCircle(eB, eA);
        //m.normal *= -1;
        return m;
    }

    Manifold checkEdgeCircle(Entity &eA, Entity &eB)
    {
        Edge &a = static_cast<Edge &>(eA);
        Circle &b = static_cast<Circle &>(eB);

        Manifold m;
        m.a = &a;
        m.b = &b;

        float side = glm::dot(b.getPosition() - a.getPosition(), a.getNormal());

        m.isColliding = std::abs(side) < b.getRadius();

        if (!m.isColliding)
            return m;

        float dotA = glm::dot(b.getPosition() - a.getVertexA(), a.getVertexB() - a.getVertexA());
        float dotB = glm::dot(b.getPosition() - a.getVertexB(), a.getVertexA() - a.getVertexB());

        if (dotA <= 0.0f)
        {
            auto normal = a.getVertexA() - b.getPosition();
            float penetration = b.getRadius() - glm::length(normal);

            if (glm::length2(normal) > std::pow(b.getRadius(), 2))
            {
                m.isColliding = false;
                return m;
            }

            m.contact_points.push_back(a.getVertexA());
            m.normal = -glm::normalize(normal);
            m.penetration = penetration;
        }
        else if (dotB <= 0.0f)
        {
            auto normal = a.getVertexB() - b.getPosition();
            float penetration = b.getRadius() - glm::length(normal);

            if (glm::length2(normal) > std::pow(b.getRadius(), 2))
            {
                m.isColliding = false;
                return m;
            }

            m.contact_points.push_back(a.getVertexB());
            m.normal = -glm::normalize(normal);
            m.penetration = penetration;
        }
        else
        {
            float d = std::abs(side);

            if (side >= 0)
            {
                m.normal = a.getNormal();
                m.penetration = b.getRadius() - d;
            }
            else if (side < 0)
            {
                m.normal = -a.getNormal();
                m.penetration = b.getRadius() - d;
            }

            m.contact_points.push_back(b.getPosition() - m.normal * d);
        }

        return m;
    }

    Manifold checkEdgeEdge(Entity &eA, Entity &eB)
    {
        return Manifold{
            .a = &eA,
            .b = &eB,
            .isColliding = false,
        };
    }

    void resolveCollision(Manifold &m, float gravityStrength)
    {
        if (m.a->getInvMass() + m.b->getInvMass() == 0)
            return;

        for (auto contact : m.contact_points)
        {
            glm::vec2 rA = contact - m.a->getPosition();
            glm::vec2 rB = contact - m.b->getPosition();

            auto rel_v = (m.b->getVelocity() + glm::cross(m.b->getAngularVelocity(), rB)) -
                         (m.a->getVelocity() + glm::cross(m.a->getAngularVelocity(), rA));

            auto norm_v = glm::dot(rel_v, m.normal);

            if (glm::length(rel_v) == 0)
                return;

            // Objects are moving apart from each other
            if (norm_v > 0)
                return;

            float aAnglMag = glm::cross(rA, m.normal);
            float bAnglMag = glm::cross(rB, m.normal);
            float totalInvMass = m.a->getInvMass() + m.b->getInvMass() +
                                 m.a->getInvInertia() * std::pow(aAnglMag, 2) +
                                 m.b->getInvInertia() * std::pow(bAnglMag, 2);

            auto e = std::min(m.a->getRestitution(), m.b->getRestitution());

            // If only gravity plays part in this collision
            // we make it a completely inelastic collision
            if (glm::length(rel_v) < gravityStrength + 0.0001f)
                e = 0;

            auto j = -(1 + e) * norm_v;
            j /= totalInvMass;
            j /= (float)m.contact_points.size();

            auto impulse = j * m.normal;
            m.a->applyImpulse(-impulse, rA);
            m.b->applyImpulse(impulse, rB);

            rel_v = (m.b->getVelocity() + glm::cross(m.b->getAngularVelocity(), rB)) -
                    (m.a->getVelocity() + glm::cross(m.a->getAngularVelocity(), rA));

            auto tangent = rel_v - (m.normal * glm::dot(rel_v, m.normal));

            if (glm::length(tangent) == 0)
                return;

            tangent = glm::normalize(tangent);

            auto jT = -glm::dot(rel_v, tangent);
            jT /= totalInvMass;
            jT /= (float)m.contact_points.size();

            if (std::abs(jT) <= 0.00001f)
                return;

            float mu = std::sqrt(std::pow(m.a->getStaticFriction(), 2) + std::pow(m.b->getStaticFriction(), 2));

            glm::vec2 friction_impulse;

            if (std::abs(jT) < j * mu)
                friction_impulse = jT * tangent;
            else
            {
                float kinetic_friction = std::sqrt(std::pow(m.a->getKineticFriction(), 2) + std::pow(m.b->getKineticFriction(), 2));
                friction_impulse = -j * tangent * kinetic_friction;
            }

            m.a->applyImpulse(-friction_impulse, rA);
            m.b->applyImpulse(friction_impulse, rB);
        }
    }
} // namespace Physics