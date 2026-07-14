#pragma once

#include <string>
#include <vector>

#include "engine/math/Rect.hpp"
#include "engine/math/Vec2.hpp"
#include "engine/tilemap/Tileset.hpp"

namespace engine {

struct TileLayer {
    std::string name;
    bool above = false;
    std::vector<int> gids;
};

struct SpawnPoint {
    std::string type;
    Vec2 position;
};

class Tilemap {
public:
    int width = 0;
    int height = 0;
    int tileWidth = 16;
    int tileHeight = 16;

    std::vector<TileLayer> layers;
    std::vector<Tileset> tilesets;
    std::vector<SpawnPoint> spawnPoints;
    std::vector<bool> solidGrid;

    int pixelWidth() const { return width * tileWidth; }
    int pixelHeight() const { return height * tileHeight; }

    const Tileset* tilesetForGid(int gid) const {
        for (const Tileset& tileset : tilesets) {
            if (tileset.containsGid(gid)) {
                return &tileset;
            }
        }
        return nullptr;
    }

    bool isSolidAt(const Rect& worldRect) const {
        if (worldRect.x < 0.0f || worldRect.y < 0.0f ||
            worldRect.x + worldRect.width > static_cast<float>(pixelWidth()) ||
            worldRect.y + worldRect.height > static_cast<float>(pixelHeight())) {
            return true;
        }

        int minTileX = static_cast<int>(worldRect.x) / tileWidth;
        int maxTileX = static_cast<int>(worldRect.x + worldRect.width - 0.001f) / tileWidth;
        int minTileY = static_cast<int>(worldRect.y) / tileHeight;
        int maxTileY = static_cast<int>(worldRect.y + worldRect.height - 0.001f) / tileHeight;

        for (int ty = minTileY; ty <= maxTileY; ++ty) {
            for (int tx = minTileX; tx <= maxTileX; ++tx) {
                if (solidGrid[static_cast<std::size_t>(ty) * width + tx]) {
                    return true;
                }
            }
        }
        return false;
    }
};

}
