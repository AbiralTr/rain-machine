#pragma once

namespace engine {

class Registry;

class AnimationSystem {
public:
    static void update(Registry& registry, float dt);
};

}
