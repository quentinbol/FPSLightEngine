#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

Camera::Camera(glm::vec3 pos) : position(pos), yaw(-90.0f), pitch(0.0f), front(0,0,-1), up(0,1,0) {}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspect) {
    return glm::perspective(glm::radians(70.0f), aspect, 0.1f, 1000.0f);
}

void Camera::addYaw(float delta) {
    yaw += delta;
    updateVectors();
}

void Camera::addPitch(float delta) {
    pitch += delta;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    updateVectors();
}

void Camera::setYawPitchFromInput(const MouseOffset& offset) {
    addYaw(offset.x * 0.1f);
    addPitch(offset.y * 0.1f);
}

void Camera::updateVectors() {
    glm::vec3 f;
    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    f.y = sin(glm::radians(pitch));
    f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(f);
}
