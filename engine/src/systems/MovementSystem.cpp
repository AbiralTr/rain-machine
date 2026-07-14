#include "engine/systems/MovementSystem.hpp"

namespace engine {

void MovementSystem::update(Registry& registry, float dt) {
    auto entities = registry.velocities().entities();
    auto velocities = registry.velocities().components();

    for (std::size_t i = 0; i < entities.size(); ++i) {
        Entity entity = entities[i];
        if (registry.colliders().has(entity) || !registry.transforms().has(entity)) {
            continue;
        }
        registry.transforms().get(entity).position += velocities[i].value * dt;
    }
}

}
