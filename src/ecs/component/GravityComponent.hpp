#pragma once
#include <glm/glm.hpp>

struct Rigidbody {
    glm::vec3 velocity{0.0f};
    glm::vec3 acceleration{0.0f};
    
    float mass = 1.0f;
    float drag = 0.98f;
    
    bool useGravity = true;
    bool isGrounded = false;
    bool isKinematic = false;

    float maxFallSpeed = 20.0f;
    float maxHorizontalSpeed = 10.0f;
};