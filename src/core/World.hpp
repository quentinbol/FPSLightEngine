#pragma once
#include <vector>
#include <unordered_map>
#include "../ecs/registry/Registry.hpp"
#include "../ecs/component/TransformComponent.hpp"
#include "../ecs/component/ShaderComponent.hpp"
#include "../ecs/component/ColliderComponent.hpp"
#include "../ecs/component/PlayerComponent.hpp"
#include "../ecs/component/LightComponent.hpp"
#include "../ecs/component/MeshComponent.hpp"
#include "../ecs/component/ColorComponent.hpp"
#include "../ecs/component/GravityComponent.hpp"

class World {
private:
    Registry registry;
    Entity playerEntity;

public:
    World() {
        // Sol
        Entity plane = registry.createEntity();
        Mesh planeMesh = Mesh::makePlane(30, 30);
        registry.addComponent<Transform>(plane, {glm::vec3(0,-0.5f,0), glm::vec3(0,0,0), glm::vec3(15,1,15)});
        registry.addComponent<Mesh>(plane, planeMesh);
        registry.addComponent<Shader>(plane, {"../shaders/vertex_shader.glsl", "../shaders/light_fragment_shader.glsl"});
        registry.addComponent<Color>(plane, {glm::vec3(0.3f, 0.7f, 0.3f)});
        registry.addComponent<Collider>(plane, {glm::vec3(15.0f, 0.5f, 15.0f)});

        // Cubes avec gravité
        Entity cube = registry.createEntity();
        registry.addComponent<Transform>(cube, {glm::vec3(0,5,0), glm::vec3(0,0,0), glm::vec3(1,1,1)});
        registry.addComponent<Mesh>(cube, Mesh::makeCube());
        registry.addComponent<Shader>(cube, {"../shaders/vertex_shader.glsl", "../shaders/light_fragment_shader.glsl"});
        registry.addComponent<Color>(cube, {glm::vec3(1.0f, 0.3f, 0.3f)});
        registry.addComponent<Collider>(cube, {Mesh::makeCube().getHalfExtents()});
        registry.addComponent<Rigidbody>(cube, Rigidbody{
            .velocity = glm::vec3(0.0f),
            .mass = 1.0f,
            .drag = 0.98f,
            .useGravity = true
        });

        Entity cube2 = registry.createEntity();
        registry.addComponent<Transform>(cube2, {glm::vec3(3,8,-2), glm::vec3(0,0,0), glm::vec3(1.5f,1.5f,1.5f)});
        registry.addComponent<Mesh>(cube2, Mesh::makeCube());
        registry.addComponent<Shader>(cube2, {"../shaders/vertex_shader.glsl", "../shaders/light_fragment_shader.glsl"});
        registry.addComponent<Color>(cube2, {glm::vec3(0.3f, 0.3f, 1.0f)});
        registry.addComponent<Collider>(cube2, {Mesh::makeCube().getHalfExtents()});
        registry.addComponent<Rigidbody>(cube2, Rigidbody{
            .velocity = glm::vec3(0.0f),
            .mass = 2.0f,
            .useGravity = true
        });

        // Plateformes fixes (pas de gravité)
        Entity platform1 = registry.createEntity();
        registry.addComponent<Transform>(platform1, {glm::vec3(-5,2,0), glm::vec3(0,0,0), glm::vec3(3,0.3f,3)});
        registry.addComponent<Mesh>(platform1, Mesh::makeCube());
        registry.addComponent<Shader>(platform1, {"../shaders/vertex_shader.glsl", "../shaders/light_fragment_shader.glsl"});
        registry.addComponent<Color>(platform1, {glm::vec3(0.8f, 0.6f, 0.2f)});
        registry.addComponent<Collider>(platform1, {glm::vec3(3.0f, 0.3f, 3.0f)});

        Entity platform2 = registry.createEntity();
        registry.addComponent<Transform>(platform2, {glm::vec3(5,4,-3), glm::vec3(0,0,0), glm::vec3(3,0.3f,3)});
        registry.addComponent<Mesh>(platform2, Mesh::makeCube());
        registry.addComponent<Shader>(platform2, {"../shaders/vertex_shader.glsl", "../shaders/light_fragment_shader.glsl"});
        registry.addComponent<Color>(platform2, {glm::vec3(0.8f, 0.6f, 0.2f)});
        registry.addComponent<Collider>(platform2, {glm::vec3(3.0f, 0.3f, 3.0f)});

        // Pyramide
        Entity pyramide = registry.createEntity();
        registry.addComponent<Transform>(pyramide, {glm::vec3(-5,0.5,5), glm::vec3(0,0,0), glm::vec3(2,2,2)});
        registry.addComponent<Mesh>(pyramide, Mesh::makePyramid());
        registry.addComponent<Shader>(pyramide, {"../shaders/vertex_shader.glsl", "../shaders/light_fragment_shader.glsl"});
        registry.addComponent<Color>(pyramide, {glm::vec3(0.8f, 0.3f, 0.8f)});
        registry.addComponent<Collider>(pyramide, {Mesh::makePyramid().getHalfExtents()});

        // Joueur avec physique
        Entity player = registry.createEntity();
        registry.addComponent<Transform>(player, {glm::vec3(0.0f, 3.0f, 5.0f), glm::vec3(0,0,0), glm::vec3(1.0f)}); 
        registry.addComponent<Collider>(player, {glm::vec3(0.4f, 0.9f, 0.4f)});
        registry.addComponent<Player>(player, {}); 
        registry.addComponent<Rigidbody>(player, Rigidbody{
            .velocity = glm::vec3(0.0f),
            .mass = 70.0f,
            .drag = 0.0f,  // Pas de drag pour le joueur (contrôle précis)
            .useGravity = true,
            .maxFallSpeed = 25.0f,
            .maxHorizontalSpeed = 15.0f
        });
        playerEntity = player;

        // Lumière directionnelle (soleil) avec ombres
        Entity sun = registry.createEntity();
        registry.addComponent<Transform>(sun, { 
            glm::vec3(-1.0f, 1.0f, -1.0f),  // Direction de la lumière (normalisée automatiquement)
            glm::vec3(0), 
            glm::vec3(1) 
        });
        registry.addComponent<Light>(sun, {
            .color = glm::vec3(1.0f, 0.95f, 0.8f),
            .intensity = 0.8f,
            .enabled = true,
            .directional = true,
            .castsShadows = true,
            .shadowW = 2048,
            .shadowH = 2048,
            .nearPlane = 0.1f,
            .farPlane = 100.0f,
            .orthoSize = 30.0f
        });

        // Lumière d'appoint
        Entity fillLight = registry.createEntity();
        registry.addComponent<Transform>(fillLight, { 
            glm::vec3(5, 10, 5), 
            glm::vec3(0), 
            glm::vec3(1) 
        });
        registry.addComponent<Light>(fillLight, {
            .color = glm::vec3(0.6f, 0.7f, 1.0f),
            .intensity = 0.3f,
            .enabled = true,
            .directional = false,
            .castsShadows = false
        });
    }

    Registry& getRegistry() { return registry; }
    Entity& getPlayerEntity() { return playerEntity; }
};