#include "engine/systems/CollisionSystem.hpp"

namespace engine {

namespace {
Rect worldColliderRect(const Transform& transform, const Collider& collider) {
    return collider.bounds.offsetBy(transform.position);
}
}

void CollisionSystem::update(Registry& registry, const Tilemap& map, float dt) {
    for (Entity entity : registry.colliders().entities()) {
        if (!registry.transforms().has(entity) || !registry.velocities().has(entity)) {
            continue;
        }

        Transform& transform = registry.transforms().get(entity);
        const Collider& collider = registry.colliders().get(entity);
        Vec2 delta = registry.velocities().get(entity).value * dt;

        transform.position.x += delta.x;
        if (map.isSolidAt(worldColliderRect(transform, collider))) {
            transform.position.x -= delta.x;
        }

        transform.position.y += delta.y;
        if (map.isSolidAt(worldColliderRect(transform, collider))) {
            transform.position.y -= delta.y;
        }
    }
}

}
