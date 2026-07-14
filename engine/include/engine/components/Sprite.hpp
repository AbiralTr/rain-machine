#pragma once

#include <SDL2/SDL.h>

#include "engine/math/Vec2.hpp"
#include "engine/render/TextureHandle.hpp"

namespace engine {

struct Sprite {
    TextureHandle texture;
    SDL_Rect srcRect{0, 0, 0, 0};
    Vec2 origin;
};

}
