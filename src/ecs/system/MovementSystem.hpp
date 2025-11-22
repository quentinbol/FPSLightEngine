#pragma once
#include "../registry/Registry.hpp"
#include "../component/TransformComponent.hpp"
#include "../component/PlayerComponent.hpp"
#include "../component/GravityComponent.hpp"
#include "../../renderer/Camera.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class PlayerMovementSystem {
public:
    PlayerMovementSystem() 
        : moveSpeed(5.0f)
        , sprintMultiplier(1.8f)
        , jumpForce(8.0f)
    {}

    void processInput(GLFWwindow* window, Registry& registry, Camera* camera, float deltaTime) {
        // Trouver l'entité joueur
        auto players = registry.getEntitiesWith<Transform, Player, Rigidbody>();
        if (players.empty()) return;

        Entity player = players[0];
        Transform& transform = registry.getComponent<Transform>(player);
        Rigidbody& rb = registry.getComponent<Rigidbody>(player);

        // Direction de la caméra (pour le mouvement relatif à la vue)
        glm::vec3 forward = camera->getForward();
        glm::vec3 right = camera->getRight();
        
        // On veut que le mouvement soit seulement horizontal
        forward.y = 0.0f;
        right.y = 0.0f;
        forward = glm::normalize(forward);
        right = glm::normalize(right);

        // Calculer le vecteur de mouvement
        glm::vec3 moveDirection(0.0f);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            moveDirection += forward;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            moveDirection -= forward;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            moveDirection -= right;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            moveDirection += right;
        }

        // Normaliser pour éviter un mouvement plus rapide en diagonale
        if (glm::length(moveDirection) > 0.0f) {
            moveDirection = glm::normalize(moveDirection);
        }

        // Appliquer le sprint
        float currentSpeed = moveSpeed;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            currentSpeed *= sprintMultiplier;
        }

        // Appliquer le mouvement (seulement sur X et Z, pas Y)
        if (glm::length(moveDirection) > 0.0f) {
            // Si au sol, mouvement direct
            if (rb.isGrounded) {
                rb.velocity.x = moveDirection.x * currentSpeed;
                rb.velocity.z = moveDirection.z * currentSpeed;
            } else {
                // Dans les airs, contrôle réduit
                float airControl = 0.3f;
                rb.velocity.x += moveDirection.x * currentSpeed * airControl * deltaTime;
                rb.velocity.z += moveDirection.z * currentSpeed * airControl * deltaTime;
            }
        } else if (rb.isGrounded) {
            // Arrêt progressif quand aucune touche n'est pressée
            rb.velocity.x *= 0.8f;
            rb.velocity.z *= 0.8f;
        }

        // Saut
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && rb.isGrounded) {
            rb.velocity.y = jumpForce;
            rb.isGrounded = false;
        }

        // Mise à jour de la position de la caméra
        // On met la caméra légèrement au-dessus de la position du joueur (au niveau des yeux)
        glm::vec3 cameraPos = transform.position;
        cameraPos.y += 0.6f;  // Hauteur des yeux
        camera->setPosition(cameraPos);
    }

    void setMoveSpeed(float speed) { moveSpeed = speed; }
    void setSprintMultiplier(float mult) { sprintMultiplier = mult; }
    void setJumpForce(float force) { jumpForce = force; }

private:
    float moveSpeed;
    float sprintMultiplier;
    float jumpForce;
};