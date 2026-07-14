find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
find_package(nlohmann_json REQUIRED)

add_library(engine STATIC
    src/core/Window.cpp
    src/core/Application.cpp
    src/render/TextureManager.cpp
    src/tilemap/TiledLoader.cpp
    src/systems/InputSystem.cpp
    src/systems/MovementSystem.cpp
    src/systems/CollisionSystem.cpp
    src/systems/CameraSystem.cpp
    src/systems/RenderSystem.cpp
)

target_include_directories(engine PUBLIC include)
target_link_libraries(engine PUBLIC PkgConfig::SDL2 PkgConfig::SDL2_IMAGE nlohmann_json::nlohmann_json)
