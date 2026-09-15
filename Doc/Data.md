# Data — technical reference

World data. Read by the Engine (`DataFactory`, `WorldFactory`), edited by the
Editor and served by the Server to the Client through `/data/manifest` and
`/data/{path}`.

## Layout

```
Data/
├── Config.json            { "ActiveFolder": "TestMap" }  — which map is active
└── <map>/                 e.g. TestMap/
    ├── Map.json           name, Width/Height (in tiles), catalogs, floor list
    ├── Tile.json          tile catalog   { "Tiles":   [ { Type, Name, TextureFolder, IsWalkable, IsAnimated?, FrameDurationMs? } ] }
    ├── Object.json        object catalog { "Objects": [ { Type, Name, Footprint{Width,Height}, TextureFolder, IsAnimated?, FrameDurationMs? } ] }
    ├── Floors/
    │   ├── 0.json         { "Z": 0, "Tiles": [[...]], "Objects": [[...]] }  — Height×Width matrices of Type
    │   └── 1.json
    └── Textures/
        ├── Tiles/<Name>/<Name>.png (or .gif when IsAnimated)
        └── Objects/<Name>/... (same)
```

## Rules

- `Type` is the numeric identifier used in the floor matrices.
  `0` in `Tiles` is the `Void` tile; `0` in `Objects` means "no object".
- `TextureFolder` is relative to the map folder.
- `IsWalkable` (default `true`) controls whether the server allows moving
  onto that tile. Always written explicitly by `TileFactory` — avoid
  relying on the default when hand-editing.
- `IsAnimated: true` makes the texture load as `<Name>.gif` (all frames
  pre-decoded at load time); without the field (or `false`) it is a static
  `<Name>.png`. `FrameDurationMs` (default 100) sets the duration of each
  animation frame.
- Sizes come from `Engine::WorldConstants`: 32 px tiles, 32-tile chunks.
- One floor per file in `Floors/`; the Editor saves only the loaded floors.
- The Server is the source of truth: the Client downloads everything via the
  manifest on the `SyncPage` and must not edit this folder. The manifest
  (`Engine::ManifestFactory`) is an MD5 hash per file, relative to this
  folder (e.g. `TestMap/Textures/Tiles/Grass/Grass.png`); the Client caches
  its copy per connected server under the OS app-data folder (see
  [`MMORPGClient.md`](MMORPGClient.md)) and only re-downloads files whose
  hash changed.
