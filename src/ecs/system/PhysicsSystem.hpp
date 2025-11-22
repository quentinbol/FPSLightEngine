#pragma once
#include "../registry/Registry.hpp"
#include "../component/TransformComponent.hpp"
#include "../component/ColliderComponent.hpp"
#include "../component/GravityComponent.hpp"
#include <glm/glm.hpp>
#include <algorithm>

class PhysicsSystem {
public:
    PhysicsSystem() 
        : gravity(0.0f, -20.0f, 0.0f)
    {}

    void update(Registry& registry, float deltaTime) {
        for (auto entity : registry.getEntitiesWith<Transform, Rigidbody>()) {
            Transform& transform = registry.getComponent<Transform>(entity);
            Rigidbody& rb = registry.getComponent<Rigidbody>(entity);

            if (rb.isKinematic) continue;

            if (rb.useGravity && !rb.isGrounded) {
                rb.acceleration += gravity;
            }

            rb.velocity += rb.acceleration * deltaTime;

            if (rb.velocity.y < -rb.maxFallSpeed) {
                rb.velocity.y = -rb.maxFallSpeed;
            }

            float horizontalSpeed = glm::length(glm::vec2(rb.velocity.x, rb.velocity.z));
            if (horizontalSpeed > rb.maxHorizontalSpeed) {
                float scale = rb.maxHorizontalSpeed / horizontalSpeed;
                rb.velocity.x *= scale;
                rb.velocity.z *= scale;
            }

            rb.velocity.x *= rb.drag;
            rb.velocity.z *= rb.drag;

            transform.position += rb.velocity * deltaTime;

            rb.acceleration = glm::vec3(0.0f);

            if (registry.hasComponent<Collider>(entity)) {
                checkGroundCollision(registry, entity, transform, rb);
            }
        }
    }

    void setGravity(const glm::vec3& newGravity) {
        gravity = newGravity;
    }

    glm::vec3 getGravity() const {
        return gravity;
    }

private:
    glm::vec3 gravity;

    void checkGroundCollision(Registry& registry, Entity entity, Transform& transform, Rigidbody& rb) {
        Collider& collider = registry.getComponent<Collider>(entity);
        
        rb.isGrounded = false;

        for (auto other : registry.getEntitiesWith<Transform, Collider>()) {
            if (other == entity) continue;

            Transform& otherTransform = registry.getComponent<Transform>(other);
            Collider& otherCollider = registry.getComponent<Collider>(other);

            glm::vec3 entityMin = transform.position - collider.halfExtents * transform.scale;
            glm::vec3 entityMax = transform.position + collider.halfExtents * transform.scale;
            
            glm::vec3 otherMin = otherTransform.position - otherCollider.halfExtents * otherTransform.scale;
            glm::vec3 otherMax = otherTransform.position + otherCollider.halfExtents * otherTransform.scale;

            bool onTop = (entityMin.y <= otherMax.y + 0.1f) && 
                         (entityMin.y >= otherMax.y - 0.5f) &&
                         (entityMax.x > otherMin.x && entityMin.x < otherMax.x) &&
                         (entityMax.z > otherMin.z && entityMin.z < otherMax.z);

            if (onTop && rb.velocity.y <= 0.0f) {
                transform.position.y = otherMax.y + collider.halfExtents.y * transform.scale.y;
                rb.velocity.y = 0.0f;
                rb.isGrounded = true;
                break;
            }

            bool collision = (entityMax.x > otherMin.x && entityMin.x < otherMax.x) &&
                            (entityMax.y > otherMin.y && entityMin.y < otherMax.y) &&
                            (entityMax.z > otherMin.z && entityMin.z < otherMax.z);

            if (collision) {
                float overlapX = std::min(entityMax.x - otherMin.x, otherMax.x - entityMin.x);
                float overlapY = std::min(entityMax.y - otherMin.y, otherMax.y - entityMin.y);
                float overlapZ = std::min(entityMax.z - otherMin.z, otherMax.z - entityMin.z);

                if (overlapX < overlapY && overlapX < overlapZ) {
                    transform.position.x += (entityMin.x < otherMin.x) ? -overlapX : overlapX;
                    rb.velocity.x = 0.0f;
                } else if (overlapY < overlapZ) {
                    transform.position.y += (entityMin.y < otherMin.y) ? -overlapY : overlapY;
                    rb.velocity.y = 0.0f;
                    if (entityMin.y < otherMin.y) {
                        rb.isGrounded = false;
                    } else {
                        rb.isGrounded = true;
                    }
                } else {
                    transform.position.z += (entityMin.z < otherMin.z) ? -overlapZ : overlapZ;
                    rb.velocity.z = 0.0f;
                }
            }
        }
    }
};