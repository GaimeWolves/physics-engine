#include <physics/Edge.hpp>

namespace Physics
{
    Edge::Edge(glm::vec2 a, glm::vec2 b)
        : m_point_a(a), m_point_b(b)
    {
        m_type = Type::Edge;

        auto facing = (b - a);
        m_position = a + facing * 0.5f;

        // Rotate 90 degrees
        float x = facing.x;
        facing.x = facing.y;
        facing.y = -x;

        m_edge_normal = glm::normalize(facing);

        updateBoundingBox();
    }

    void Edge::render()
    {
        auto rgb = color::convert::to_rgb(m_color);

        glLineWidth(2.5f);
        glBegin(GL_LINES);
        glColor3f(rgb.r, rgb.g, rgb.b);
        glVertex2f(m_point_a.x, m_point_a.y);
        glVertex2f(m_point_b.x, m_point_b.y);
        glEnd();
        glLineWidth(1.0f);
    }

    void Edge::updateBoundingBox()
    {
        m_boundingBox.set(
            std::min(m_point_a.x, m_point_b.x), std::min(m_point_a.y, m_point_b.y),
            std::max(m_point_a.x, m_point_b.x), std::max(m_point_a.y, m_point_b.y));
    }
} // namespace Physics
