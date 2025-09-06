#pragma once
#include <glm/glm.hpp>
#include "./MeshComponent.hpp"

struct Collider {
    glm::vec3 halfExtents;

    Collider() : halfExtents(0.5f, 0.5f, 0.5f) {}

    Collider(const glm::vec3& he) : halfExtents(he) {}
};