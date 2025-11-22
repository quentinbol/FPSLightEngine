#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Light {
    glm::vec3 color {1.0f, 1.0f, 1.0f};
    float intensity = 1.0f;
    bool enabled = true;

    bool directional = true;
    bool castsShadows = true;

    GLuint depthFBO = 0;
    GLuint depthTex = 0;
    int shadowW = 1024, shadowH = 1024;
    float nearPlane = 1.0f, farPlane = 50.0f;
    float orthoSize = 20.0f;
};
