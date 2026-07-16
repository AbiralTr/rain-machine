#include "engine/systems/AnimationSystem.hpp"

#include <cmath>
#include <cstddef>

#include "engine/components/Animation.hpp"
#include "engine/ecs/Registry.hpp"
#include "engine/math/Vec2.hpp"

namespace engine {

namespace {
constexpr float kFrameDuration = 0.15f;
constexpr float kMovingSpeedThreshold = 1.0f;
}

void AnimationSystem::update(Registry& registry, float dt) {
    for (Entity entity : registry.animations().entities()) {
        if (!registry.velocities().has(entity) || !registry.sprites().has(entity)) {
            continue;
        }

        Animation& anim = registry.animations().get(entity);
        Vec2 velocity = registry.velocities().get(entity).value;
        bool moving = velocity.length() > kMovingSpeedThreshold;

        if (moving) {
            if (std::abs(velocity.x) > std::abs(velocity.y)) {
                anim.facing = velocity.x > 0.0f ? Direction::Right : Direction::Left;
            } else {
                anim.facing = velocity.y > 0.0f ? Direction::Down : Direction::Up;
            }
        }

        if (moving != anim.moving) {
            anim.walkFrameIndex = 0;
            anim.frameTimer = 0.0f;
        }
        anim.moving = moving;

        const DirectionFrames& dirFrames = anim.frames[static_cast<std::size_t>(anim.facing)];
        TextureHandle chosen = dirFrames.idle;

        if (moving) {
            anim.frameTimer += dt;
            while (anim.frameTimer >= kFrameDuration) {
                anim.frameTimer -= kFrameDuration;
                anim.walkFrameIndex = (anim.walkFrameIndex + 1) % static_cast<int>(dirFrames.walk.size());
            }
            chosen = dirFrames.walk[static_cast<std::size_t>(anim.walkFrameIndex)];
        }

        registry.sprites().get(entity).texture = chosen;
    }
}

}
