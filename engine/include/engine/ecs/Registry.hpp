#pragma once

#include <vector>

#include "engine/components/Animation.hpp"
#include "engine/components/Collider.hpp"
#include "engine/components/PlayerControlled.hpp"
#include "engine/components/Sprite.hpp"
#include "engine/components/Transform.hpp"
#include "engine/components/Velocity.hpp"
#include "engine/ecs/ComponentPool.hpp"
#include "engine/ecs/Entity.hpp"

namespace engine {

class Registry {
public:
    Entity createEntity() {
        if (!freeIndices_.empty()) {
            std::uint32_t index = freeIndices_.back();
            freeIndices_.pop_back();
            return Entity{index, generations_[index]};
        }
        std::uint32_t index = static_cast<std::uint32_t>(generations_.size());
        generations_.push_back(0);
        return Entity{index, 0};
    }

    void destroyEntity(Entity entity) {
        transforms_.remove(entity);
        sprites_.remove(entity);
        velocities_.remove(entity);
        colliders_.remove(entity);
        playerControlled_.remove(entity);
        animations_.remove(entity);

        generations_[entity.index]++;
        freeIndices_.push_back(entity.index);
    }

    ComponentPool<Transform>& transforms() { return transforms_; }
    ComponentPool<Sprite>& sprites() { return sprites_; }
    ComponentPool<Velocity>& velocities() { return velocities_; }
    ComponentPool<Collider>& colliders() { return colliders_; }
    ComponentPool<PlayerControlled>& playerControlled() { return playerControlled_; }
    ComponentPool<Animation>& animations() { return animations_; }

private:
    std::vector<std::uint32_t> generations_;
    std::vector<std::uint32_t> freeIndices_;

    ComponentPool<Transform> transforms_;
    ComponentPool<Sprite> sprites_;
    ComponentPool<Velocity> velocities_;
    ComponentPool<Collider> colliders_;
    ComponentPool<PlayerControlled> playerControlled_;
    ComponentPool<Animation> animations_;
};

}
