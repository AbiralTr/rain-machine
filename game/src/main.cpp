#include <iostream>

#include "engine/core/Application.hpp"

int main() {
    engine::Application app;

    if (!app.init("assets/maps/test_map.tmj", "assets/sprites/player.png")) {
        std::cerr << "Failed to initialize application.\n";
        return 1;
    }

    app.run();
    return 0;
}
