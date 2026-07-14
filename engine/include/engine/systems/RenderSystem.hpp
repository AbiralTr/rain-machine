#pragma once

#include <SDL2/SDL.h>

#include "engine/ecs/Registry.hpp"
#include "engine/render/Camera.hpp"
#include "engine/render/TextureManager.hpp"
#include "engine/tilemap/Tilemap.hpp"

namespace engine {

class RenderSystem {
public:
    static void render(SDL_Renderer* renderer, Registry& registry, const Tilemap& map,
                        const Camera& camera, TextureManager& textures);
};

}
