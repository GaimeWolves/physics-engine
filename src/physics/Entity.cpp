#include <physics/Entity.hpp>

#include <util.hpp>

namespace Physics
{
    void Entity::update(float dt)
    {
        if (m_mass == PHYSICS_INF_MASS)
            return;

        if (!m_awake)
            return;

        m_acceleration = m_force * m_inv_mass;
        m_velocity += m_acceleration * dt;

        m_position += m_velocity * dt;

        m_angular_velocity += m_torque * m_inv_inertia * dt;
        m_orientation += m_angular_velocity * dt;

        updateBoundingBox();
    }

    void Entity::applyImpulse(const glm::vec2 &impulse, const glm::vec2 &contact)
    {
        m_velocity += m_inv_mass * impulse;
        m_angular_velocity += m_inv_inertia * glm::cross(contact, impulse);
    }

} // namespace Physics
