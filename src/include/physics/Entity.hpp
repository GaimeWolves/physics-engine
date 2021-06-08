#pragma once

#include <iostream>

#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <physics/general.hpp>
#include <engine/color.hpp>
#include <physics/Manifold.hpp>
#include <physics/definitions.hpp>
#include <physics/collisions.hpp>

namespace Physics
{
    class Entity
    {
    public:
        enum class Type
        {
            Circle,
            Edge,
        };

        virtual void update(float dt);

        virtual void render() = 0;
        virtual Manifold collidesWith(Entity &other) { return isColliding(*this, other); };

        virtual void applyForceToCenter(const glm::vec2 &force) { m_force += force; }
        virtual void applyImpulse(const glm::vec2 &impulse, const glm::vec2 &contact);
        void clearForces() { m_force = {0, 0}; }

        void move(glm::vec2 displacement) { m_position += displacement; }
        void wake() { m_awake = true; }

        AABB getBoundingBox() const { return m_boundingBox; }
        glm::vec2 getPosition() const { return m_position; }
        glm::vec2 getVelocity() const { return m_velocity; }
        glm::vec2 getAcceleration() const { return m_acceleration; }
        glm::vec2 getForce() const { return m_force; }
        float getOrientation() const { return m_orientation; }
        float getAngularVelocity() const { return m_angular_velocity; }
        float getTorque() const { return m_torque; }
        float getMass() const { return m_mass; }
        float getInvMass() const { return m_inv_mass; }
        float getInertia() const { return m_inertia; }
        float getInvInertia() const { return m_inv_inertia; }
        float getRestitution() const { return m_restitution; }
        float getStaticFriction() const { return m_friction_static; }
        float getKineticFriction() const { return m_friction_kinetic; }
        Type getType() const { return m_type; }

        void setPosition(glm::vec2 position) { m_position = position; }
        void setVelocity(glm::vec2 velocity) { m_velocity = velocity; }
        void setColor(color::hsl color) { m_color = color; }
        void setStaticFriction(float friction) { m_friction_static = friction; }
        void setKineticFriction(float friction) { m_friction_kinetic = friction; }

        virtual void setMass(float mass) = 0;

    protected:
        virtual void updateBoundingBox() = 0;

        AABB m_boundingBox{0, 0, 0, 0};

        glm::vec2 m_position{0, 0};
        glm::vec2 m_velocity{0, 0};
        glm::vec2 m_acceleration{0, 0};
        glm::vec2 m_force{0, 0};

        float m_orientation{0};
        float m_angular_velocity{0};
        float m_torque{0};

        float m_mass{1};
        float m_inv_mass{1};

        float m_inertia{1};
        float m_inv_inertia{1};

        float m_restitution{0.75f};

        float m_friction_static{0.5f};
        float m_friction_kinetic{0.4f};

        color::hsl m_color{0, 1.0f, 0.0f};
        bool m_awake = true;

        Type m_type{-1};
    };
}; // namespace Physics