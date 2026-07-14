#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "engine/render/TextureHandle.hpp"

namespace engine {

class TextureManager {
public:
    explicit TextureManager(SDL_Renderer* renderer);
    ~TextureManager();

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    TextureHandle load(const std::string& path);
    SDL_Texture* get(TextureHandle handle) const;

private:
    SDL_Renderer* renderer_;
    std::unordered_map<std::string, TextureHandle> pathToHandle_;
    std::vector<SDL_Texture*> textures_;
};

}
