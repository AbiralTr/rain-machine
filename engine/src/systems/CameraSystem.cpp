#include "engine/systems/CameraSystem.hpp"

#include <algorithm>

namespace engine {

namespace {
float clampAxis(float desired, int mapPixels, int viewportPixels) {
    if (mapPixels <= viewportPixels) {
        return static_cast<float>(mapPixels - viewportPixels) / 2.0f;
    }
    return std::clamp(desired, 0.0f, static_cast<float>(mapPixels - viewportPixels));
}
}

void CameraSystem::update(Camera& camera, Registry& registry, const Tilemap& map) {
    if (!registry.transforms().has(camera.target)) {
        return;
    }

    Vec2 targetPos = registry.transforms().get(camera.target).position;
    float desiredX = targetPos.x - static_cast<float>(camera.viewportWidth) / 2.0f;
    float desiredY = targetPos.y - static_cast<float>(camera.viewportHeight) / 2.0f;

    camera.position.x = clampAxis(desiredX, map.pixelWidth(), camera.viewportWidth);
    camera.position.y = clampAxis(desiredY, map.pixelHeight(), camera.viewportHeight);
}

}
