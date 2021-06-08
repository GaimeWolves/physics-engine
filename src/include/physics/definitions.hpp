#pragma once

#define PHYSICS_INF_MASS 0
#define PHYSICS_SLEEP_EPSILON 0.00005f
#define PHYSICS_WAKE_EPSILON 0.004f

namespace Physics
{
    class Entity;
    class Circle;
    class Edge;

    struct Manifold;
} // namespace Physics