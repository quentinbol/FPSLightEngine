#pragma once
#include "../registry/Registry.hpp"
#include "../component/TransformComponent.hpp"
#include "../component/MeshComponent.hpp"
#include "../component/ShaderComponent.hpp"
#include "../component/LightComponent.hpp"
#include "../component/ColorComponent.hpp"
#include "../../renderer/Camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static constexpr Entity INVALID_ENTITY_VAL = std::numeric_limits<Entity>::max();

class RenderSystem {
public:
    RenderSystem()
        : depthShader("../shaders/depth_vs.glsl", "../shaders/depth_fs.glsl")
    {}

    void update(Registry &registry, Camera *camera, int width, int height) {
        Entity shadowLight = findShadowCastingLight(registry);
        glm::mat4 lightSpace = glm::mat4(1.0f);
        bool hasShadows = false;

        if (shadowLight != INVALID_ENTITY_VAL) {
            Transform& lt = registry.getComponent<Transform>(shadowLight);
            Light& light = registry.getComponent<Light>(shadowLight);

            ensureShadowResources(light);

            float s = light.orthoSize;
            glm::mat4 lightProj = glm::ortho(-s, s, -s, s, light.nearPlane, light.farPlane);

            glm::vec3 target = camera->getPosition();
            glm::vec3 lightDir = glm::normalize(lt.position);
            glm::vec3 lightPos = target + lightDir * (light.farPlane * 0.5f);
            
            glm::mat4 lightView = glm::lookAt(lightPos, target, glm::vec3(0, 1, 0));
            lightSpace = lightProj * lightView;

            glViewport(0, 0, light.shadowW, light.shadowH);
            glBindFramebuffer(GL_FRAMEBUFFER, light.depthFBO);
            glClear(GL_DEPTH_BUFFER_BIT);

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(2.0f, 4.0f);

            glCullFace(GL_FRONT);
            glEnable(GL_CULL_FACE);

            depthShader.use();
            depthShader.setMat4("lightSpaceMatrix", lightSpace);

            for (auto e : registry.getEntitiesWith<Transform, Mesh>()) {
                Transform& t = registry.getComponent<Transform>(e);
                Mesh& m = registry.getComponent<Mesh>(e);

                glm::mat4 model(1.0f);
                model = glm::translate(model, t.position);
                model = glm::rotate(model, glm::radians(t.rotation.x), glm::vec3(1, 0, 0));
                model = glm::rotate(model, glm::radians(t.rotation.y), glm::vec3(0, 1, 0));
                model = glm::rotate(model, glm::radians(t.rotation.z), glm::vec3(0, 0, 1));
                model = glm::scale(model, t.scale);

                depthShader.setMat4("model", model);
                m.draw();
            }

            glDisable(GL_POLYGON_OFFSET_FILL);
            glCullFace(GL_BACK);
            glDisable(GL_CULL_FACE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            hasShadows = true;
        }

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 proj = camera->getProjectionMatrix((float)width / (float)height);

        auto lights = registry.getEntitiesWith<Transform, Light>();

        for (auto e : registry.getEntitiesWith<Transform, Mesh, Shader>()) {
            Transform& t = registry.getComponent<Transform>(e);
            Mesh& m = registry.getComponent<Mesh>(e);
            Shader& sh = registry.getComponent<Shader>(e);

            sh.use();

            glm::mat4 model(1.0f);
            model = glm::translate(model, t.position);
            model = glm::rotate(model, glm::radians(t.rotation.x), glm::vec3(1, 0, 0));
            model = glm::rotate(model, glm::radians(t.rotation.y), glm::vec3(0, 1, 0));
            model = glm::rotate(model, glm::radians(t.rotation.z), glm::vec3(0, 0, 1));
            model = glm::scale(model, t.scale);

            sh.setMat4("model", model);
            sh.setMat4("view", view);
            sh.setMat4("projection", proj);
            sh.setVec3("viewPos", camera->getPosition());

            glm::vec3 objColor = glm::vec3(1.0f);
            if (registry.hasComponent<Color>(e)) {
                Color& c = registry.getComponent<Color>(e);
                objColor = c.value;
            }
            sh.setVec3("objectColor", objColor);

            int n = 0;
            for (auto le : lights) {
                if (n >= 16) break;
                Transform& lt = registry.getComponent<Transform>(le);
                Light& li = registry.getComponent<Light>(le);
                if (!li.enabled) continue;

                std::string base = "lights[" + std::to_string(n) + "]";
                sh.setVec3(base + ".position", lt.position);
                sh.setVec3(base + ".color", li.color);
                sh.setFloat(base + ".intensity", li.intensity);
                n++;
            }
            sh.setInt("numLights", n);

            if (hasShadows) {
                Light& li = registry.getComponent<Light>(shadowLight);
                sh.setMat4("lightSpaceMatrix", lightSpace);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, li.depthTex);
                sh.setInt("shadowMap", 0);

                sh.setInt("hasShadows", 1);
            } else {
                sh.setInt("hasShadows", 0);
            }

            m.draw();
        }

        // Unbind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    Shader depthShader;

    Entity findShadowCastingLight(Registry& registry) {
        for (auto e : registry.getEntitiesWith<Transform, Light>()) {
            Light& l = registry.getComponent<Light>(e);
            if (l.enabled && l.castsShadows) return e;
        }
        return INVALID_ENTITY_VAL;
    }

    void ensureShadowResources(Light& l) {
        if (l.depthFBO != 0 && l.depthTex != 0) return;

        glGenFramebuffers(1, &l.depthFBO);

        glGenTextures(1, &l.depthTex);
        glBindTexture(GL_TEXTURE_2D, l.depthTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                     l.shadowW, l.shadowH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        float border[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

        glBindFramebuffer(GL_FRAMEBUFFER, l.depthFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, l.depthTex, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Erreur: Shadow framebuffer incomplet!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};