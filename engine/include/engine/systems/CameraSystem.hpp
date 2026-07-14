#pragma once

#include "engine/ecs/Registry.hpp"
#include "engine/render/Camera.hpp"
#include "engine/tilemap/Tilemap.hpp"

namespace engine {

class CameraSystem {
public:
    static void update(Camera& camera, Registry& registry, const Tilemap& map);
};

}
