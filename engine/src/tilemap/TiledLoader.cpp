#include "engine/tilemap/TiledLoader.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

namespace engine {

namespace {

using json = nlohmann::json;

bool equalsIgnoreCase(const std::string& a, const std::string& b) {
    return a.size() == b.size() &&
           std::equal(a.begin(), a.end(), b.begin(), [](unsigned char ca, unsigned char cb) {
               return std::tolower(ca) == std::tolower(cb);
           });
}

bool readAboveProperty(const json& layer) {
    if (!layer.contains("properties")) {
        return false;
    }
    for (const json& property : layer["properties"]) {
        if (property.value("name", "") == "above") {
            return property.value("value", false);
        }
    }
    return false;
}

void rasterizeRectSolid(std::vector<bool>& solidGrid, int mapWidth, int mapHeight, int tileWidth,
                         int tileHeight, float x, float y, float w, float h) {
    int minTileX = std::max(0, static_cast<int>(x) / tileWidth);
    int maxTileX = std::min(mapWidth - 1, static_cast<int>(x + w - 0.001f) / tileWidth);
    int minTileY = std::max(0, static_cast<int>(y) / tileHeight);
    int maxTileY = std::min(mapHeight - 1, static_cast<int>(y + h - 0.001f) / tileHeight);

    for (int ty = minTileY; ty <= maxTileY; ++ty) {
        for (int tx = minTileX; tx <= maxTileX; ++tx) {
            solidGrid[static_cast<std::size_t>(ty) * mapWidth + tx] = true;
        }
    }
}

}

std::optional<Tilemap> TiledLoader::load(const std::string& jsonPath, TextureManager& textures) {
    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        std::cerr << "TiledLoader: could not open " << jsonPath << "\n";
        return std::nullopt;
    }

    json root;
    try {
        file >> root;
    } catch (const json::parse_error& e) {
        std::cerr << "TiledLoader: failed to parse " << jsonPath << ": " << e.what() << "\n";
        return std::nullopt;
    }

    if (root.value("infinite", false)) {
        std::cerr << "TiledLoader: infinite maps are not supported (" << jsonPath << ")\n";
        return std::nullopt;
    }

    Tilemap map;
    map.width = root.value("width", 0);
    map.height = root.value("height", 0);
    map.tileWidth = root.value("tilewidth", 16);
    map.tileHeight = root.value("tileheight", 16);
    map.solidGrid.assign(static_cast<std::size_t>(map.width) * map.height, false);

    std::filesystem::path baseDir = std::filesystem::path(jsonPath).parent_path();

    for (const json& tilesetJson : root.value("tilesets", json::array())) {
        if (!tilesetJson.contains("image")) {
            std::cerr << "TiledLoader: external (non-embedded) tilesets are not supported\n";
            return std::nullopt;
        }

        Tileset tileset;
        tileset.firstGid = tilesetJson.value("firstgid", 1);
        tileset.tileWidth = tilesetJson.value("tilewidth", map.tileWidth);
        tileset.tileHeight = tilesetJson.value("tileheight", map.tileHeight);
        tileset.columns = tilesetJson.value("columns", 1);
        tileset.tileCount = tilesetJson.value("tilecount", 0);

        std::filesystem::path imagePath = baseDir / tilesetJson["image"].get<std::string>();
        tileset.texture = textures.load(imagePath.string());

        map.tilesets.push_back(tileset);
    }

    for (const json& layerJson : root.value("layers", json::array())) {
        std::string type = layerJson.value("type", "");
        std::string name = layerJson.value("name", "");

        if (type == "tilelayer") {
            if (layerJson.contains("data") && layerJson["data"].is_string()) {
                std::cerr << "TiledLoader: compressed/base64 tile layers are not supported ("
                          << name << ")\n";
                return std::nullopt;
            }

            std::vector<int> gids = layerJson.value("data", std::vector<int>{});

            if (equalsIgnoreCase(name, "collision")) {
                for (int i = 0; i < static_cast<int>(gids.size()); ++i) {
                    if (gids[i] != 0) {
                        map.solidGrid[static_cast<std::size_t>(i)] = true;
                    }
                }
                continue;
            }

            TileLayer layer;
            layer.name = name;
            layer.above = readAboveProperty(layerJson);
            layer.gids = std::move(gids);
            map.layers.push_back(std::move(layer));

        } else if (type == "objectgroup") {
            bool isCollisionLayer = equalsIgnoreCase(name, "collision");

            for (const json& object : layerJson.value("objects", json::array())) {
                float x = object.value("x", 0.0f);
                float y = object.value("y", 0.0f);
                float w = object.value("width", 0.0f);
                float h = object.value("height", 0.0f);

                if (isCollisionLayer) {
                    rasterizeRectSolid(map.solidGrid, map.width, map.height, map.tileWidth,
                                        map.tileHeight, x, y, w, h);
                    continue;
                }

                std::string objectType = object.value("type", "");
                if (objectType.empty()) {
                    objectType = object.value("class", "");
                }
                if (!objectType.empty()) {
                    map.spawnPoints.push_back(SpawnPoint{objectType, Vec2{x, y}});
                }
            }
        }
    }

    return map;
}

}
