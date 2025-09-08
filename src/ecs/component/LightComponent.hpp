#pragma once
#include <glm/glm.hpp>

struct Light {
    glm::vec3 color;
    float intensity;
    u_int lightNumber;
    bool enabled;

    Light(const glm::vec3 &c = glm::vec3(1.0f), float i = 1.0f, u_int ln = 0, bool e = true)
        : color(c), intensity(i), lightNumber(ln), enabled(e) {}
};
