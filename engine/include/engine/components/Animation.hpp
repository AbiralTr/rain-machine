#pragma once

#include <array>

#include "engine/render/TextureHandle.hpp"

namespace engine {

enum class Direction { Down, Up, Left, Right };

struct DirectionFrames {
    TextureHandle idle;
    std::array<TextureHandle, 2> walk;
};

struct Animation {
    std::array<DirectionFrames, 4> frames;
    Direction facing = Direction::Down;
    bool moving = false;
    int walkFrameIndex = 0;
    float frameTimer = 0.0f;
};

}
