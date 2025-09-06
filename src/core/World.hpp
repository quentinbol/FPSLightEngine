#pragma once
#include <vector>
#include <unordered_map>
#include "../ecs/registry/Registry.hpp"
#include "../ecs/component/TransformComponent.hpp"
#include "../ecs/component/ShaderComponent.hpp"
#include "../ecs/component/ColliderComponent.hpp"
#include "../ecs/component/PlayerComponent.hpp"

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
        registry.addComponent<Shader>(plane, {"../shaders/vertex_shader.glsl", "../shaders/blue_fragment_shader.glsl"});
        registry.addComponent<Collider>(plane, {glm::vec3(15.0f, 0.01f, 15.0f)});

        Entity cube = registry.createEntity();
        registry.addComponent<Transform>(cube, {glm::vec3(0,0,0), glm::vec3(1,1,1)});
        registry.addComponent<Mesh>(cube, Mesh::makeCube());
        registry.addComponent<Shader>(cube, {"../shaders/vertex_shader.glsl", "../shaders/red_fragment_shader.glsl"});
        registry.addComponent<Collider>(cube, {Mesh::makeCube().getHalfExtents()});

        Entity cube2 = registry.createEntity();
        registry.addComponent<Transform>(cube2, {glm::vec3(2,5,0), glm::vec3(1,1,1), glm::vec3(2,2,2)});
        registry.addComponent<Mesh>(cube2, Mesh::makeCube());
        registry.addComponent<Shader>(cube2, {"../shaders/vertex_shader.glsl", "../shaders/red_fragment_shader.glsl"});

        Entity cube3 = registry.createEntity();
        registry.addComponent<Transform>(cube3, {glm::vec3(-2,7,0), glm::vec3(1,1,1), glm::vec3(2,2,2)});
        registry.addComponent<Mesh>(cube3, Mesh::makeCube());
        registry.addComponent<Shader>(cube3, {"../shaders/vertex_shader.glsl", "../shaders/red_fragment_shader.glsl"});

        Entity player = registry.createEntity();
        registry.addComponent<Transform>(player, {glm::vec3(0.0f, 1.6f, 3.0f), glm::vec3(1.0f)}); 
        registry.addComponent<Collider>(player, {glm::vec3(0.3f, 0.9f, 0.3f)});
        registry.addComponent<Player>(player, {}); 
        playerEntity = player;
    }

    Registry& getRegistry() { return registry; }
    Entity& getPlayerEntity() { return playerEntity; }
};
