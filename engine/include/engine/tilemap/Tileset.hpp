#pragma once

#include <SDL2/SDL.h>

#include "engine/render/TextureHandle.hpp"

namespace engine {

struct Tileset {
    TextureHandle texture;
    int firstGid = 1;
    int tileWidth = 16;
    int tileHeight = 16;
    int columns = 1;
    int tileCount = 0;

    bool containsGid(int gid) const { return gid >= firstGid && gid < firstGid + tileCount; }

    SDL_Rect sourceRectFor(int gid) const {
        int localId = gid - firstGid;
        int col = columns > 0 ? localId % columns : 0;
        int row = columns > 0 ? localId / columns : 0;
        return SDL_Rect{col * tileWidth, row * tileHeight, tileWidth, tileHeight};
    }
};

}
