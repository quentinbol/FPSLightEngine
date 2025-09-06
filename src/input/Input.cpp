#include "Input.hpp"
#include <GLFW/glfw3.h>


Input::Input(GLFWwindow* window) : lastX(640), lastY(360), firstMouse(true), xoffset(0), yoffset(0) {
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
        Input* self = (Input*)glfwGetWindowUserPointer(win);

        if (self->firstMouse) {
            self->lastX = xpos;
            self->lastY = ypos;
            self->firstMouse = false;
        }

        self->xoffset = xpos - self->lastX;
        self->yoffset = self->lastY - ypos;
        self->lastX = xpos;
        self->lastY = ypos;
    });
}

void Input::processInput(GLFWwindow* window, Registry& registry, Entity player, float deltaTime) {
    Transform& transform = registry.getComponent<Transform>(player);
    Player& playerComp = registry.getComponent<Player>(player);

    glm::vec3 forward = camera->getForward(); 
    glm::vec3 right   = camera->getRight();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        transform.position += forward * playerComp.speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        transform.position -= forward * playerComp.speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        transform.position -= right * playerComp.speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        transform.position += right * playerComp.speed * deltaTime;
}
