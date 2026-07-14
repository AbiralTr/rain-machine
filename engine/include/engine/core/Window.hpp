#pragma once

#include <SDL2/SDL.h>

#include <string>

namespace engine {

struct WindowConfig {
    std::string title = "rain-machine";
    int width = 960;
    int height = 540;
    bool vsync = true;
};

class Window {
public:
    explicit Window(const WindowConfig& config);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    SDL_Renderer* renderer() const { return renderer_; }

private:
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
};

}
