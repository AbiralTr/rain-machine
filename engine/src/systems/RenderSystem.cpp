#include "engine/systems/RenderSystem.hpp"

#include <algorithm>
#include <vector>

namespace engine {

namespace {

struct Drawable {
    Entity entity;
    float sortY;
};

std::vector<Drawable> collectDrawables(Registry& registry) {
    std::vector<Drawable> drawables;
    for (Entity entity : registry.sprites().entities()) {
        if (!registry.transforms().has(entity)) {
            continue;
        }
        const Transform& transform = registry.transforms().get(entity);
        const Sprite& sprite = registry.sprites().get(entity);
        float feetY = transform.position.y - sprite.origin.y + sprite.srcRect.h;
        drawables.push_back({entity, feetY});
    }
    std::sort(drawables.begin(), drawables.end(),
              [](const Drawable& a, const Drawable& b) { return a.sortY < b.sortY; });
    return drawables;
}

void drawSprite(SDL_Renderer* renderer, Registry& registry, Entity entity, const Camera& camera,
                 TextureManager& textures) {
    const Transform& transform = registry.transforms().get(entity);
    const Sprite& sprite = registry.sprites().get(entity);
    if (!sprite.texture.isValid()) {
        return;
    }

    SDL_Rect dst{static_cast<int>(transform.position.x - sprite.origin.x - camera.position.x),
                 static_cast<int>(transform.position.y - sprite.origin.y - camera.position.y),
                 sprite.srcRect.w, sprite.srcRect.h};
    SDL_RenderCopy(renderer, textures.get(sprite.texture), &sprite.srcRect, &dst);
}

void drawTileRow(SDL_Renderer* renderer, const Tilemap& map, const TileLayer& layer, int ty,
                  int startTx, int endTx, const Camera& camera, TextureManager& textures) {
    for (int tx = startTx; tx <= endTx; ++tx) {
        int gid = layer.gids[static_cast<std::size_t>(ty) * map.width + tx];
        if (gid == 0) {
            continue;
        }

        const Tileset* tileset = map.tilesetForGid(gid);
        if (tileset == nullptr) {
            continue;
        }

        SDL_Rect src = tileset->sourceRectFor(gid);
        SDL_Rect dst{tx * map.tileWidth - static_cast<int>(camera.position.x),
                     ty * map.tileHeight - static_cast<int>(camera.position.y), map.tileWidth,
                     map.tileHeight};
        SDL_RenderCopy(renderer, textures.get(tileset->texture), &src, &dst);
    }
}

void drawTileLayer(SDL_Renderer* renderer, const Tilemap& map, const TileLayer& layer,
                    const Camera& camera, TextureManager& textures) {
    int startTx = std::max(0, static_cast<int>(camera.position.x) / map.tileWidth);
    int endTx = std::min(map.width - 1,
                          (static_cast<int>(camera.position.x) + camera.viewportWidth) / map.tileWidth);
    int startTy = std::max(0, static_cast<int>(camera.position.y) / map.tileHeight);
    int endTy = std::min(
        map.height - 1, (static_cast<int>(camera.position.y) + camera.viewportHeight) / map.tileHeight);

    for (int ty = startTy; ty <= endTy; ++ty) {
        drawTileRow(renderer, map, layer, ty, startTx, endTx, camera, textures);
    }
}

}

void RenderSystem::render(SDL_Renderer* renderer, Registry& registry, const Tilemap& map,
                           const Camera& camera, TextureManager& textures) {
    std::vector<const TileLayer*> belowLayers;
    std::vector<const TileLayer*> aboveLayers;
    for (const TileLayer& layer : map.layers) {
        (layer.above ? aboveLayers : belowLayers).push_back(&layer);
    }

    std::vector<Drawable> drawables = collectDrawables(registry);
    std::size_t nextDrawable = 0;

    int startTx = std::max(0, static_cast<int>(camera.position.x) / map.tileWidth);
    int endTx = std::min(map.width - 1,
                          (static_cast<int>(camera.position.x) + camera.viewportWidth) / map.tileWidth);
    int startTy = std::max(0, static_cast<int>(camera.position.y) / map.tileHeight);
    int endTy = std::min(
        map.height - 1, (static_cast<int>(camera.position.y) + camera.viewportHeight) / map.tileHeight);

    for (int ty = startTy; ty <= endTy; ++ty) {
        for (const TileLayer* layer : belowLayers) {
            drawTileRow(renderer, map, *layer, ty, startTx, endTx, camera, textures);
        }

        float rowBottom = static_cast<float>((ty + 1) * map.tileHeight);
        while (nextDrawable < drawables.size() && drawables[nextDrawable].sortY < rowBottom) {
            drawSprite(renderer, registry, drawables[nextDrawable].entity, camera, textures);
            ++nextDrawable;
        }
    }

    while (nextDrawable < drawables.size()) {
        drawSprite(renderer, registry, drawables[nextDrawable].entity, camera, textures);
        ++nextDrawable;
    }

    for (const TileLayer* layer : aboveLayers) {
        drawTileLayer(renderer, map, *layer, camera, textures);
    }
}

}
