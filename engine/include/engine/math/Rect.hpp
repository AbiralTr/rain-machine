#pragma once

#include "engine/math/Vec2.hpp"

namespace engine {

struct Rect {
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;

    bool intersects(const Rect& other) const {
        return x < other.x + other.width && x + width > other.x &&
               y < other.y + other.height && y + height > other.y;
    }

    Rect offsetBy(const Vec2& offset) const { return {x + offset.x, y + offset.y, width, height}; }
};

}
