#pragma once
#include "../registry/Registry.hpp"
#include "../component/LightComponent.hpp"
#include "../component/TransformComponent.hpp"
#include "../component/ShaderComponent.hpp"

class LightSystem {
public:
    void update(Registry &registry, Camera *camera) {
        auto lights = registry.getEntitiesWith<Transform, Light>();

        for (auto entity : registry.getEntitiesWith<Shader>()) {
            Shader &shader = registry.getComponent<Shader>(entity);
            shader.use();

            int lightIndex = 0;
            for (auto lightEntity : lights) {
                Transform &lightTransform = registry.getComponent<Transform>(lightEntity);
                Light &light = registry.getComponent<Light>(lightEntity);

                if (!light.enabled) continue;

                std::string base = "lights[" + std::to_string(lightIndex) + "]";
                shader.setVec3(base + ".position", lightTransform.position);
                shader.setVec3(base + ".color", light.color);
                shader.setFloat(base + ".intensity", light.intensity);

                lightIndex++;
            }
            shader.setVec3("viewPos", camera->getPosition());
            shader.setInt("numLights", (int)lights.size());
        }
    }
};
