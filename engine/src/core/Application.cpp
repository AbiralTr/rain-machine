#include "engine/core/Application.hpp"

#include <SDL2/SDL.h>

#include <algorithm>
#include <iostream>

#include "engine/components/Animation.hpp"
#include "engine/components/Collider.hpp"
#include "engine/components/PlayerControlled.hpp"
#include "engine/components/Sprite.hpp"
#include "engine/components/Transform.hpp"
#include "engine/components/Velocity.hpp"
#include "engine/core/Time.hpp"
#include "engine/systems/AnimationSystem.hpp"
#include "engine/systems/CameraSystem.hpp"
#include "engine/systems/CollisionSystem.hpp"
#include "engine/systems/InputSystem.hpp"
#include "engine/systems/MovementSystem.hpp"
#include "engine/systems/RenderSystem.hpp"
#include "engine/tilemap/TiledLoader.hpp"

namespace engine {

namespace {
constexpr double kFixedDt = 1.0 / 60.0;
constexpr double kMaxFrameTime = 0.25;

TextureHandle loadFrame(TextureManager& textures, const std::string& dir, const std::string& name) {
    return textures.load(dir + "/" + name + ".png");
}

DirectionFrames loadDirectionFrames(TextureManager& textures, const std::string& dir,
                                     const std::string& direction) {
    return DirectionFrames{loadFrame(textures, dir, direction + "_idle"),
                            {loadFrame(textures, dir, direction + "_walk1"),
                             loadFrame(textures, dir, direction + "_walk2")}};
}
}

Application::Application() = default;
Application::~Application() = default;

bool Application::init(const std::string& mapPath, const std::string& playerSpriteDir) {
    try {
        window_ = std::make_unique<Window>(WindowConfig{});
    } catch (const std::exception& e) {
        std::cerr << "Application::init: " << e.what() << "\n";
        return false;
    }

    SDL_RenderSetLogicalSize(window_->renderer(), camera_.viewportWidth, camera_.viewportHeight);

    textures_ = std::make_unique<TextureManager>(window_->renderer());

    std::optional<Tilemap> loadedMap = TiledLoader::load(mapPath, *textures_);
    if (!loadedMap) {
        std::cerr << "Application::init: failed to load map " << mapPath << "\n";
        return false;
    }
    map_ = std::move(*loadedMap);

    Vec2 spawnPosition;
    bool foundSpawn = false;
    for (const SpawnPoint& spawn : map_.spawnPoints) {
        if (spawn.type == "player_spawn") {
            spawnPosition = spawn.position;
            foundSpawn = true;
            break;
        }
    }
    if (!foundSpawn) {
        std::cerr << "Application::init: no player_spawn object found, defaulting to (0, 0)\n";
    }

    Animation playerAnimation;
    try {
        playerAnimation.frames[static_cast<std::size_t>(Direction::Down)] =
            loadDirectionFrames(*textures_, playerSpriteDir, "down");
        playerAnimation.frames[static_cast<std::size_t>(Direction::Up)] =
            loadDirectionFrames(*textures_, playerSpriteDir, "up");
        playerAnimation.frames[static_cast<std::size_t>(Direction::Left)] =
            loadDirectionFrames(*textures_, playerSpriteDir, "left");
        playerAnimation.frames[static_cast<std::size_t>(Direction::Right)] =
            loadDirectionFrames(*textures_, playerSpriteDir, "right");
    } catch (const std::exception& e) {
        std::cerr << "Application::init: " << e.what() << "\n";
        return false;
    }

    TextureHandle initialTexture = playerAnimation.frames[static_cast<std::size_t>(Direction::Down)].idle;

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(textures_->get(initialTexture), nullptr, nullptr, &texW, &texH);

    playerEntity_ = registry_.createEntity();
    registry_.transforms().emplace(playerEntity_, Transform{spawnPosition});
    registry_.velocities().emplace(playerEntity_, Velocity{});
    registry_.sprites().emplace(
        playerEntity_, Sprite{initialTexture, SDL_Rect{0, 0, texW, texH}, Vec2{texW / 2.0f, texH / 2.0f}});

    float colliderWidth = texW * 0.5f;
    float colliderHeight = texH * 0.3f;
    registry_.colliders().emplace(
        playerEntity_,
        Collider{Rect{(texW - colliderWidth) / 2.0f - texW / 2.0f, texH / 2.0f - colliderHeight,
                      colliderWidth, colliderHeight}});
    registry_.playerControlled().emplace(playerEntity_, PlayerControlled{});
    registry_.animations().emplace(playerEntity_, std::move(playerAnimation));

    camera_.target = playerEntity_;

    running_ = true;
    return true;
}

void Application::run() {
    Clock::time_point previous = Clock::now();
    double accumulator = 0.0;

    while (running_) {
        Clock::time_point current = Clock::now();
        double frameTime = std::min(secondsBetween(previous, current), kMaxFrameTime);
        previous = current;
        accumulator += frameTime;

        pumpEvents();

        while (accumulator >= kFixedDt) {
            fixedUpdate(static_cast<float>(kFixedDt));
            accumulator -= kFixedDt;
        }

        render();
    }
}

void Application::pumpEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running_ = false;
        }
    }
}

void Application::fixedUpdate(float dt) {
    InputSystem::update(registry_);
    MovementSystem::update(registry_, dt);
    CollisionSystem::update(registry_, map_, dt);
    CameraSystem::update(camera_, registry_, map_);
    AnimationSystem::update(registry_, dt);
}

void Application::render() {
    SDL_SetRenderDrawColor(window_->renderer(), 0, 0, 0, 255);
    SDL_RenderClear(window_->renderer());

    RenderSystem::render(window_->renderer(), registry_, map_, camera_, *textures_);

    SDL_RenderPresent(window_->renderer());
}

}
