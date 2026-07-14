#include "engine/systems/InputSystem.hpp"

#include <SDL2/SDL.h>

namespace engine {

namespace {
constexpr float kPlayerSpeed = 80.0f;
}

void InputSystem::update(Registry& registry) {
    const Uint8* keys = SDL_GetKeyboardState(nullptr);

    Vec2 direction;
    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP]) direction.y -= 1.0f;
    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN]) direction.y += 1.0f;
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]) direction.x -= 1.0f;
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) direction.x += 1.0f;

    Vec2 velocity = direction.normalized() * kPlayerSpeed;

    for (Entity entity : registry.playerControlled().entities()) {
        if (registry.velocities().has(entity)) {
            registry.velocities().get(entity).value = velocity;
        }
    }
}

}
