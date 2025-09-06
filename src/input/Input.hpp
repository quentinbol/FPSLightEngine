#pragma once
#include "../renderer/Camera.hpp"
#include "../ecs/registry/Registry.hpp"
#include "../ecs/component/TransformComponent.hpp"
#include "../ecs/component/PlayerComponent.hpp"

class GLFWwindow;

class Input {
    public:
        Input(GLFWwindow *window);
        void processInput(GLFWwindow *window, Registry &registry, Entity player, float deltaTime = 0.016f);
        MouseOffset getMouseOffset() {
            MouseOffset offset = { xoffset, yoffset };
            xoffset = 0;
            yoffset = 0;
            return offset;
        }
        void setCamera(Camera *cam) { camera = cam; }
    private:
        float lastX, lastY;
        float xoffset, yoffset;
        bool firstMouse;
        Camera *camera;
};