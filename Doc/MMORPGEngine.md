# MMORPGEngine — technical reference

Static library (`MMORPG::MMORPGEngine`) shared by Server, Client and Editor.
Everything lives in `namespace Engine`. It has no executable of its own.

Depends on Qt 6 (Core, Network, Qml, Quick, WebSockets) and jsoncpp. Always
include by full path: `#include <MMORPGEngine/World/WorldModel.h>`.

## Structure

| Folder       | Contents                                                                                     |
| ------------ | --------------------------------------------------------------------------------------------- |
| `Account/`   | `AccountModel`/`AccountCharacterModel` (runtime) and their `*DTO` (network)                 |
| `Commons/`   | `JsonHelper` (JSON load/save via jsoncpp, plus `writeJsonString` for outbound messages), `Singleton<T>`, `RegisterEngineTypes` (registers `Viewport` in QML as the `MMORPGEngine` module) |
| `Data/`      | `DataManager` (singleton: catalogs + `configPath()`), `DataFactory` (reads/writes `Config.json`, `Tile.json`, `Object.json`), `Manifest/` (`ManifestFactory` walks `Config.json` + the active map folder computing an MD5 per file — used by the Server to build `/data/manifest` and by the Client to hash its local cache), `Tile/` (`TileCatalog`, `TileModel` — includes `isWalkable`), `Object/` (`ObjectCatalog`, `ObjectModel`, `ObjectSizeModel`), `Animation/` (`AnimationModel` — pre-decoded frames of a texture, static or `.gif`) |
| `Entity/`    | `EntityModel` + `Position`/`Orientation`/`Size`; `Character/` (`CharacterModel`, `CharacterPositionModel`) and `Creature/` specializations; `EntityStateDTO` (network: idCharacter + x/y/z + worldName, used by `/ws/character`) |
| `Network/`   | `HttpClient` (REST) and `WebSocketClient` (thin `QWebSocket` wrapper — header-based `X-Session` auth, `messageReceived`/`errorOccurred` signals), both used by the Client to talk to the Server |
| `Renderer/`  | `Renderer` → `TileRenderer` + `ObjectRenderer` + `EntityRenderer`; `Camera/` (world↔screen, zoom, clamp); `Scene/` (`RenderScene`, `RenderSceneItem`); `World/RenderWorld` (base, specialized per module, exposes `object`/`tile`/`entities` per floor) |
| `UI/`        | `Viewport` — `QQuickItem` that draws a `RenderWorld` through the scene graph                 |
| `World/`     | `WorldModel` → floors → `ChunkModel` → `WorldTileModel`/`WorldObjectModel`; `WorldFactory` (load/save of `Map.json` + `Floors/*.json`); `WorldConstants` |

## Concepts

- **Constants**: `CHUNK_SIZE = 32` tiles, `TILE_SIZE = 32` px
  (`World/WorldConstants.h`).
- **Floors (Z)**: `WorldModel` tracks the known floors
  (`floors()`/`addFloor()`). `floors()` reflects what is *loaded* — in the
  Editor/Server that is the whole map; in the Client, eventually, only the
  chunks near the player. Do not assume it enumerates the whole world.
- **Tile vs Object**: a tile is the ground (one per cell); an object sits on
  top and `type 0` means "no object". Objects have no orientation yet. Tiles
  have an `isWalkable` flag (default `true`); objects don't block movement
  yet.
- **Catalog vs world**: `TileModel`/`ObjectModel` (in `Data/`) are the
  available *types*; `WorldTileModel`/`WorldObjectModel` (in `World/`) are
  the *instances* placed on the map, referencing the type by `Type`.
- **Animated textures**: `TileModel`/`ObjectModel` hold an `AnimationModel`
  instead of a single `QImage`. `texture()` still returns the first frame
  (thumbnails/preview); renderers use `animation().frameAt(elapsedMs)` to
  get the current frame. `Viewport` has an internal `QTimer` (100 ms) that
  forces a redraw so animations play without depending on another event.
- **Model vs DTO**: `*Model` is runtime; `*DTO` is the format that travels
  over the network/JSON.
- **Entities**: `RenderWorld::entities(z)` returns the `RenderWorld::Entity`
  (id + x/y) list for a floor. `EntityRenderer` only draws the ones inside
  the camera's visible range, same culling `ObjectRenderer`/`TileRenderer`
  already do. Each module's `RenderWorld` decides where its entities come
  from: `ClientRenderWorld` holds its own (pushed via `setEntity`/
  `removeEntity`), `ServerRenderWorld` reads live from `WorldManager`,
  `EditorRenderWorld` has none yet.

## Viewport

`Engine::Viewport` (`UI/Viewport/`) is the QML render component, used by all
three modules:

- `renderWorld`: the `RenderWorld` to draw (each module passes its own
  specialization).
- `cameraPosition`, `centerCameraOnTile()`, `moveCameraByTiles()`; the
  `Camera` clamps the position to the world bounds (`setWorldSize`/`clampPosition`).
- `activeFloor`: the floor (Z) being drawn. `Renderer` (and the tile/object/
  entity renderers it owns) receive `z` as a parameter — never hard-code
  `z = 0`. Who sets the value depends on the module: Editor/Server through
  the UI, Client through the authoritative Z of the player's entity.
- `setHighlightedTile()`/`clearHighlight()`: highlight of the tile under the
  mouse.
- Signal `tileClicked(x, y, z)`.

## Data flow

```
Data/Config.json ──► DataFactory ──► DataManager (TileCatalog, ObjectCatalog)
Data/Config.json + <map>/**  ──► ManifestFactory ──► ManifestModel (path + MD5 hash per file)
Data/<map>/Map.json + Floors/*.json ──► WorldFactory ──► WorldModel
WorldModel + catalogs ──► RenderWorld ──► Renderer ──► Viewport (QML)
```
