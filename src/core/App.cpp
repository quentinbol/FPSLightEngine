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
    physicsSystem = new PhysicsSystem();
    playerMovementSystem = new PlayerMovementSystem();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    lastFrameTime = glfwGetTime();
}

App::~App() {
    delete renderer;
    delete collisionSystem;
    delete physicsSystem;
    delete playerMovementSystem;
    delete input;
    delete camera;
    delete world;
    glfwDestroyWindow(window);
    glfwTerminate();
}

void App::run() {
    while (!glfwWindowShouldClose(window)) {
        // Calcul du deltaTime
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastFrameTime);
        lastFrameTime = currentTime;
        
        // Limiter le deltaTime pour éviter les gros sauts
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Entity player = world->getPlayerEntity();
        
        // Mise à jour des systèmes dans le bon ordre
        
        // 1. Input du joueur (applique les forces)
        playerMovementSystem->processInput(window, world->getRegistry(), camera, deltaTime);
        
        // 2. Physique (gravité, vélocité, etc.)
        physicsSystem->update(world->getRegistry(), deltaTime);
        
        // 3. Collision (déjà géré dans PhysicsSystem mais on peut garder pour d'autres vérifications)
        // collisionSystem->update(world->getRegistry());
        
        // 4. Rendu
        renderer->update(world->getRegistry(), camera, width, height);

        glfwSwapBuffers(window);
    }
}