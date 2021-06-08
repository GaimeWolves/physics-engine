#include <physics/Circle.hpp>

namespace Physics
{
    Circle::Circle(glm::vec2 position, float radius)
        : m_radius(radius)
    {
        m_position = position;
        m_type = Type::Circle;
        updateBoundingBox();
    }

    void Circle::render()
    {
        auto rgb = color::convert::to_rgb(m_color);

        glLineWidth(2.0f);
        glBegin(GL_LINE_STRIP);
        glColor3f(rgb.r, rgb.g, rgb.b);
        glVertex2f(m_position.x, m_position.y);
        float angle = m_orientation;
        glm::vec2 start = glm::vec2(m_position.x + sin(-angle) * m_radius, m_position.y + cos(-angle) * m_radius);
        for (; angle < m_orientation + M_PI * 2; angle += 0.1)
            glVertex2f(m_position.x + sin(-angle) * m_radius, m_position.y + cos(-angle) * m_radius);
        glVertex2f(start.x, start.y);
        glEnd();
        glLineWidth(1.0f);
    }

    void Circle::updateBoundingBox()
    {
        m_boundingBox.set(
            m_position.x - m_radius, m_position.y - m_radius,
            m_position.x + m_radius, m_position.y + m_radius);
    }
} // namespace Physics