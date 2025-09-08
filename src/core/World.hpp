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

class World {
private:
    Registry registry;
    Entity playerEntity;

public:
    World() {

        Entity plane = registry.createEntity();
        Mesh planeMesh = Mesh::makePlane(30, 30);
        registry.addComponent<Transform>(plane, {glm::vec3(0,0,0), glm::vec3(1,1,1), glm::vec3(3,3,3)});
        registry.addComponent<Mesh>(plane, planeMesh);
        registry.addComponent<Shader>(plane, {"../shaders/vertex_shader.glsl", "../shaders/light_fragment_shader.glsl"});
        registry.addComponent<Color>(plane, {glm::vec3(1.0f, 1.0f, 1.0f)});
        registry.addComponent<Collider>(plane, {glm::vec3(15.0f, 0.01f, 15.0f)});

        Entity cube = registry.createEntity();
        registry.addComponent<Transform>(cube, {glm::vec3(0,0,0), glm::vec3(1,1,1)});
        registry.addComponent<Mesh>(cube, Mesh::makeCube());
        registry.addComponent<Shader>(cube, {"../shaders/vertex_shader.glsl", "../shaders/light_fragment_shader.glsl"});
        registry.addComponent<Color>(cube, {glm::vec3(0.0f, 0.5f, 1.0f)});
        registry.addComponent<Collider>(cube, {Mesh::makeCube().getHalfExtents()});

        Entity cube2 = registry.createEntity();
        registry.addComponent<Transform>(cube2, {glm::vec3(2,5,0), glm::vec3(1,1,1), glm::vec3(2,2,2)});
        registry.addComponent<Mesh>(cube2, Mesh::makeCube());
        registry.addComponent<Shader>(cube2, {"../shaders/vertex_shader.glsl", "../shaders/light_fragment_shader.glsl"});

        Entity cube3 = registry.createEntity();
        registry.addComponent<Transform>(cube3, {glm::vec3(-2,7,0), glm::vec3(1,1,1), glm::vec3(2,2,2)});
        registry.addComponent<Mesh>(cube3, Mesh::makeCube());
        registry.addComponent<Shader>(cube3, {"../shaders/vertex_shader.glsl", "../shaders/light_fragment_shader.glsl"});

        Entity pyramide = registry.createEntity();
        registry.addComponent<Transform>(pyramide, {glm::vec3(-5,1,0), glm::vec3(5,1,1), glm::vec3(2,2,2)});
        registry.addComponent<Mesh>(pyramide, Mesh::makePyramid());
        registry.addComponent<Shader>(pyramide, {"../shaders/vertex_shader.glsl", "../shaders/light_fragment_shader.glsl"});
        registry.addComponent<Color>(pyramide, {glm::vec3(0.0f, 1.0f, 0.0f)});
        registry.addComponent<Collider>(pyramide, {Mesh::makePyramid().getHalfExtents()});

        Entity player = registry.createEntity();
        registry.addComponent<Transform>(player, {glm::vec3(0.0f, 1.6f, 3.0f), glm::vec3(1.0f)}); 
        registry.addComponent<Collider>(player, {glm::vec3(0.3f, 0.9f, 0.3f)});
        registry.addComponent<Player>(player, {}); 
        playerEntity = player;

        Entity light = registry.createEntity();
        registry.addComponent<Transform>(light, {glm::vec3(15, 10, 15)});
        registry.addComponent<Mesh>(light, Mesh::makeCube());
        registry.addComponent<Shader>(light, {"../shaders/vertex_shader.glsl", "../shaders/blue_fragment_shader.glsl"});
        registry.addComponent<Light>(light, {glm::vec3(0.0f, 0.0f, 1.0f), 0.6f});

        Entity light2 = registry.createEntity();
        registry.addComponent<Transform>(light2, {glm::vec3(-15, 10, -15)});
        registry.addComponent<Mesh>(light2, Mesh::makeCube());
        registry.addComponent<Shader>(light2, {"../shaders/vertex_shader.glsl", "../shaders/yellow_fragment_shader.glsl"});
        registry.addComponent<Color>(light2, {glm::vec3(1.0f, 1.0f, 0.0f)});
        registry.addComponent<Light>(light2, {glm::vec3(1.0f, 1.0f, 0.0f), 0.6f});

        Entity light3 = registry.createEntity();
        registry.addComponent<Transform>(light3, {glm::vec3(0, 30, 0)});
        registry.addComponent<Mesh>(light3, Mesh::makePyramid());
        registry.addComponent<Shader>(light3, {"../shaders/vertex_shader.glsl", "../shaders/red_fragment_shader.glsl"});
        registry.addComponent<Color>(light3, {glm::vec3(1.0f, 0.0f, 0.0f)});
        registry.addComponent<Light>(light3, {glm::vec3(1.0f, 0.0f, 0.0f), 0.6f});
    }

    Registry& getRegistry() { return registry; }
    Entity& getPlayerEntity() { return playerEntity; }
};
