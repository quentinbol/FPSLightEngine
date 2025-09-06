#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct MouseOffset {
    float x;
    float y;
};

class Camera {
public:
    Camera(glm::vec3 position);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix(float aspect);

    void addYaw(float delta);
    void addPitch(float delta);

    void setYawPitchFromInput(const MouseOffset& offset);

    glm::vec3 getForward() const { return front; }
    glm::vec3 getRight()   const { return glm::normalize(glm::cross(front, up)); }
    glm::vec3 getPosition() const { return position; }
    void setPosition(const glm::vec3& pos) { position = pos; }

private:
    glm::vec3 position;
    float yaw, pitch;
    glm::vec3 front;
    glm::vec3 up;

    void updateVectors();
};
