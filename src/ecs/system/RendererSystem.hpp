#pragma once
#include "../registry/Registry.hpp"
#include "../component/TransformComponent.hpp"
#include "../component/MeshComponent.hpp"
#include "../component/ShaderComponent.hpp"
#include "../../renderer/Camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class RenderSystem {
public:
    void update(Registry &registry, Camera *camera, int width, int height) {
        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 projection = camera->getProjectionMatrix((float)width / height);

        for (auto entity : registry.getEntitiesWith<Transform, Mesh, Shader>()) {

            Transform &transform = registry.getComponent<Transform>(entity);
            Mesh &mesh = registry.getComponent<Mesh>(entity);
            Shader &shader = registry.getComponent<Shader>(entity);

            shader.use();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, transform.position);
            model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1,0,0));
            model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0,1,0));
            model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0,0,1));
            model = glm::scale(model, transform.scale);

            shader.setMat4("model", model);
            shader.setMat4("view", view);
            shader.setMat4("projection", projection);

            mesh.draw();
        }
    }
};
