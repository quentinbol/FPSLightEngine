#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Represents the transformation of an entity in 3D space
 * 
 * This includes its position, rotation, and scale.
 * 
 */
struct Transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    /**
     * @brief Construct a new Transform object
     * 
     * @param position
     * @param rotation
     * @param scale
     */
    Transform(glm::vec3 pos = {0,0,0}, glm::vec3 rot = {0,0,0}, glm::vec3 scl = {1,1,1})
        : position(pos), rotation(rot), scale(scl) {}

    /**
     * @brief Get the Model Matrix object
     * 
     * @return glm::mat4
     */
    glm::mat4 getModelMatrix() const {
        glm::mat4 model(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), {1,0,0});
        model = glm::rotate(model, glm::radians(rotation.y), {0,1,0});
        model = glm::rotate(model, glm::radians(rotation.z), {0,0,1});
        model = glm::scale(model, scale);
        return model;
    }
};