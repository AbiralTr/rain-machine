#include "engine/render/TextureManager.hpp"

#include <SDL2/SDL_image.h>

#include <stdexcept>

namespace engine {

TextureManager::TextureManager(SDL_Renderer* renderer) : renderer_(renderer) {}

TextureManager::~TextureManager() {
    for (SDL_Texture* texture : textures_) {
        SDL_DestroyTexture(texture);
    }
}

TextureHandle TextureManager::load(const std::string& path) {
    auto it = pathToHandle_.find(path);
    if (it != pathToHandle_.end()) {
        return it->second;
    }

    SDL_Texture* texture = IMG_LoadTexture(renderer_, path.c_str());
    if (texture == nullptr) {
        throw std::runtime_error("Failed to load texture '" + path + "': " + IMG_GetError());
    }

    TextureHandle handle{textures_.size()};
    textures_.push_back(texture);
    pathToHandle_.emplace(path, handle);
    return handle;
}

SDL_Texture* TextureManager::get(TextureHandle handle) const {
    return textures_.at(handle.index);
}

}
