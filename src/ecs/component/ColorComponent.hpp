#pragma once
#include <glm/glm.hpp>

struct Color {
    glm::vec3 value;

    Color(const glm::vec3 &v = glm::vec3(1.0f))
        : value(v) {}
};
