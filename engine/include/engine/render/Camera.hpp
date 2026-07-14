#pragma once

#include "engine/ecs/Entity.hpp"
#include "engine/math/Vec2.hpp"

namespace engine {

struct Camera {
    Vec2 position;
    int viewportWidth = 320;
    int viewportHeight = 180;
    Entity target = Entity::invalid();
};

}
