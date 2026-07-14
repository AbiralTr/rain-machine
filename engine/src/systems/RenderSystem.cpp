#include "engine/systems/RenderSystem.hpp"

#include <algorithm>
#include <vector>

namespace engine {

namespace {

void drawTileLayer(SDL_Renderer* renderer, const Tilemap& map, const TileLayer& layer,
                    const Camera& camera, TextureManager& textures) {
    int startTx = std::max(0, static_cast<int>(camera.position.x) / map.tileWidth);
    int endTx = std::min(map.width - 1,
                          (static_cast<int>(camera.position.x) + camera.viewportWidth) / map.tileWidth);
    int startTy = std::max(0, static_cast<int>(camera.position.y) / map.tileHeight);
    int endTy = std::min(
        map.height - 1, (static_cast<int>(camera.position.y) + camera.viewportHeight) / map.tileHeight);

    for (int ty = startTy; ty <= endTy; ++ty) {
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
}

void drawSprites(SDL_Renderer* renderer, Registry& registry, const Camera& camera,
                  TextureManager& textures) {
    struct Drawable {
        Entity entity;
        float sortY;
    };

    std::vector<Drawable> drawables;
    for (Entity entity : registry.sprites().entities()) {
        if (!registry.transforms().has(entity)) {
            continue;
        }
        drawables.push_back({entity, registry.transforms().get(entity).position.y});
    }

    std::sort(drawables.begin(), drawables.end(),
              [](const Drawable& a, const Drawable& b) { return a.sortY < b.sortY; });

    for (const Drawable& drawable : drawables) {
        const Transform& transform = registry.transforms().get(drawable.entity);
        const Sprite& sprite = registry.sprites().get(drawable.entity);
        if (!sprite.texture.isValid()) {
            continue;
        }

        SDL_Rect dst{static_cast<int>(transform.position.x - sprite.origin.x - camera.position.x),
                     static_cast<int>(transform.position.y - sprite.origin.y - camera.position.y),
                     sprite.srcRect.w, sprite.srcRect.h};
        SDL_RenderCopy(renderer, textures.get(sprite.texture), &sprite.srcRect, &dst);
    }
}

}

void RenderSystem::render(SDL_Renderer* renderer, Registry& registry, const Tilemap& map,
                           const Camera& camera, TextureManager& textures) {
    for (const TileLayer& layer : map.layers) {
        if (!layer.above) {
            drawTileLayer(renderer, map, layer, camera, textures);
        }
    }

    drawSprites(renderer, registry, camera, textures);

    for (const TileLayer& layer : map.layers) {
        if (layer.above) {
            drawTileLayer(renderer, map, layer, camera, textures);
        }
    }
}

}
