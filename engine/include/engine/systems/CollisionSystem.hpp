#pragma once

#include "engine/ecs/Registry.hpp"
#include "engine/tilemap/Tilemap.hpp"

namespace engine {

class CollisionSystem {
public:
    static void update(Registry& registry, const Tilemap& map, float dt);
};

}
