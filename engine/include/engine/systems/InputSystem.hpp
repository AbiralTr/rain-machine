#pragma once

#include "engine/ecs/Registry.hpp"

namespace engine {

class InputSystem {
public:
    static void update(Registry& registry);
};

}
