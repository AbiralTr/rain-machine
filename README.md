# rain-machine

A general-purpose 2D game engine in C++, built for GBA-era top-down action-RPGs with the style of *Dragon Ball Z: Buu's Fury*, *Legacy of Goku II*, and *The Legend of Zelda: The Minish Cap* / *Four Swords*. Layered tile depth, real-time action, 8-directional movement.

## Status

Milestone 1 in progress — core loop, ECS, Tiled map loading, collision, camera.

## Stack

- C++20, CMake
- SDL2 / SDL2_image for windowing and rendering
- [Tiled](https://www.mapeditor.org/) for level authoring (JSON export)
- nlohmann-json for parsing

## Structure

```
engine/   reusable engine library (rendering, ECS, tilemap, systems)
game/     sandbox game that consumes the engine
```

## Build

```
cmake -S . -B build
cmake --build build -j$(nproc)
./build/game/sandbox_game
```
