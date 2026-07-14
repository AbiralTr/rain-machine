# assets

`sandbox_game` expects, relative to the built executable:

- `maps/test_map.tmj` — a Tiled JSON export (not infinite, embedded tileset, CSV tile data). Layers: one or more base tile layers, an "above" tile layer (custom bool property `above` = true) for depth, an object or tile layer literally named `collision`, and an object layer with a `player_spawn`-typed object.
- `sprites/player.png` — a single-frame player sprite (no animation yet).
- `tilesets/...` — tileset image(s) referenced by the map.

None of these exist yet — running `sandbox_game` before adding them will fail to load the map and exit with an error. See the Milestone 1 plan for the exact test map spec.
