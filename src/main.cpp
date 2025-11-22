#include "core/App.hpp"

int main() {
    try {
        App app(1920, 1080, "FPS Light Engine");
        app.run();
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
}