#pragma once

#include <memory>
#include <string>

#include "engine/core/Window.hpp"
#include "engine/ecs/Registry.hpp"
#include "engine/render/Camera.hpp"
#include "engine/render/TextureManager.hpp"
#include "engine/tilemap/Tilemap.hpp"

namespace engine {

class Application {
public:
    Application();
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    bool init(const std::string& mapPath, const std::string& playerSpritePath);

    void run();

private:
    void pumpEvents();
    void fixedUpdate(float dt);
    void render();

    std::unique_ptr<Window> window_;
    std::unique_ptr<TextureManager> textures_;
    Registry registry_;
    Tilemap map_;
    Camera camera_;
    Entity playerEntity_ = Entity::invalid();
    bool running_ = false;
};

}
