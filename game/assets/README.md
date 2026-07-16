# assets

`sandbox_game` expects, relative to the process's working directory:

- `maps/test_map.tmj` — a Tiled JSON export (not infinite, embedded tileset, CSV tile data). Layers: one or more base tile layers, an "above" tile layer (custom bool property `above` = true) for depth, an object or tile layer literally named `collision`, and an object layer with a `player_spawn`-typed object.
- `sprites/player/` — 12 frames, 4 directions × (1 idle + 2 walk), named `{down,up,left,right}_{idle,walk1,walk2}.png`. All frames must be the same size.
- `tilesets/...` — tileset image(s) referenced by the map.
