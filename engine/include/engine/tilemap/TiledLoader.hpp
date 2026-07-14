#pragma once

#include <optional>
#include <string>

#include "engine/render/TextureManager.hpp"
#include "engine/tilemap/Tilemap.hpp"

namespace engine {

class TiledLoader {
public:
    static std::optional<Tilemap> load(const std::string& jsonPath, TextureManager& textures);
};

}
