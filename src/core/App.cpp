#include "App.hpp"


App::App(int width, int height, const std::string &title)
: width(width), height(height), title(title) {
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW" << std::endl;
        exit(-1);
    }

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD" << std::endl;
        exit(-1);
    }


    glEnable(GL_DEPTH_TEST);

    world = new World();
    input = new Input(window);
    camera = new Camera(glm::vec3(0.0f, 1.6f, 3.0f));
    input->setCamera(camera);
    renderer = new RenderSystem();
    collisionSystem = new CollisionSystem();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


App::~App() {
    delete renderer;
    delete collisionSystem;
    delete input;
    delete camera;
    glfwDestroyWindow(window);
    glfwTerminate();
}


void App::run() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Entity player = world->getPlayerEntity();
        Transform& playerTransform = world->getRegistry().getComponent<Transform>(player);

        camera->setPosition(playerTransform.position);

        camera->setYawPitchFromInput(input->getMouseOffset());

        input->processInput(window, world->getRegistry(), player, 0.016f);

        renderer->update(world->getRegistry(), camera, width, height);
        collisionSystem->update(world->getRegistry());

        glfwSwapBuffers(window);
    }
}
