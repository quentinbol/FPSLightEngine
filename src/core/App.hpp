#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "World.hpp"
#include "../input/Input.hpp"
#include "../renderer/Camera.hpp"
#include "../ecs/system/RendererSystem.hpp"
#include "../ecs/system/CollisionSystem.hpp"
#include "../ecs/system/PhysicsSystem.hpp"
#include "../ecs/system/MovementSystem.hpp"

class App {
private:
    GLFWwindow* window;
    int width, height;
    std::string title;
    
    World* world;
    Input* input;
    Camera* camera;
    RenderSystem* renderer;
    CollisionSystem* collisionSystem;
    PhysicsSystem* physicsSystem;
    PlayerMovementSystem* playerMovementSystem;
    
    double lastFrameTime;

public:
    App(int width, int height, const std::string &title);
    ~App();
    void run();
};