#pragma once

#include "engine/ecs/Registry.hpp"

namespace engine {

class MovementSystem {
public:
    static void update(Registry& registry, float dt);
};

}
