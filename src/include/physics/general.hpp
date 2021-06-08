#pragma once

namespace Physics
{
    class AABB
    {
    public:
        AABB() = default;
        AABB(double x1, double y1, double x2, double y2)
            : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
        {
        }

        bool intersects(const AABB &other) const
        {
            if (m_x2 < other.m_x1 || m_x1 > other.m_x2)
                return false;

            if (m_y2 < other.m_y1 || m_y1 > other.m_y2)
                return false;

            return true;
        }

        double getX1() const { return m_x1; }
        double getY1() const { return m_y1; }
        double getX2() const { return m_x2; }
        double getY2() const { return m_y2; }

        void set(double x1, double y1, double x2, double y2)
        {
            m_x1 = x1;
            m_y1 = y1;
            m_x2 = x2;
            m_y2 = y2;
        }

    private:
        double m_x1, m_y1, m_x2, m_y2;
    };
}; // namespace Physics