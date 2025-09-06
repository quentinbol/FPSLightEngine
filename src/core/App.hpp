#pragma once
#include <string>
#include <glm/glm.hpp>
#include "input/Input.hpp"
#include "renderer/Camera.hpp"
#include "../ecs/system/RendererSystem.hpp"
#include "../ecs/system/CollisionSystem.hpp"
#include "World.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


class App {
    public:
        App(int width, int height, const std::string &title);
        ~App();
        void run();


    private:
        GLFWwindow *window;
        RenderSystem *renderer;
        CollisionSystem *collisionSystem;
        Input *input;
        Camera *camera;
        int width, height;
        std::string title;
        World *world;
};