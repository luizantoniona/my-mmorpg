# MMORPGEngine

The shared core of MyMMO. A C++20 static library that the Server, the
Client and the Editor all link against, so the three applications agree on
what a world is, how it is stored, how it is drawn and how it is described
on the wire.

## What it provides

- **World model** — a map is a set of floors (Z levels), each floor a grid
  of 32×32-tile chunks. Every cell holds a ground *tile* and, optionally, an
  *object* on top of it. Type `0` means "empty" for both.
- **Data catalogs** — tile and object *types* (name, texture, walkability,
  footprint, animation) loaded from JSON and shared by every application.
  Textures can be static PNGs or animated GIFs with pre-decoded frames.
  **Item types** (`ItemTypeModel`) are a fourth catalog, deliberately
  minimal for now (`Type`, `Name` only) — just enough for a skill tree to
  reference an item type by id. Gameplay fields (equip slot, hand
  requirement, resource cost) come back once something actually reads
  them — see `DESIGN.md` for the full intended shape. **Skill trees**
  (`SkillCatalog` → `SkillTreeModel`
  → `SkillNodeModel`) are a fifth: one tree per item type, each node
  carrying `ProficiencyLevel` and `Prerequisites`, deliberately minimal
  for the same reason as item types. A tree references its item type by
  id only; loading skips any tree whose item type isn't in the item type
  catalog.
- **Entities** — position, size and vitals (health, mana, stamina) for
  characters and creatures, plus the DTOs that carry them over the network.
- **Renderer** — a Qt Quick scene-graph renderer with a camera
  (world ↔ screen, zoom, clamping to the map bounds) and per-layer
  renderers for tiles, objects and entities. It culls to the visible
  range and resolves *see-through floors*: an empty cell on the active
  floor shows the floor underneath, Tibia-style.
- **`Viewport`** — a QML item that draws any `RenderWorld`. The Editor, the
  Server's debug window and the Client all use the same component; only
  who drives the camera and the active floor changes.
- **Networking helpers** — an HTTP client, a WebSocket client with
  session-header auth, and the typed message contract for the game socket
  (separate enums for client → server and server → client messages).
- **Data manifest** — an MD5-per-file listing of the world folder, so
  clients can sync only what changed.

## Design principles

- **Model vs. DTO.** `*Model` classes are runtime state; `*DTO` classes are
  the JSON that travels. Every DTO knows how to build itself from its
  model, so no call site assembles JSON by hand.
- **Catalog vs. instance.** A `TileModel` is a *kind* of tile; a
  `WorldTileModel` is one placed on the map and references its kind by type.
- **Nothing here exists for a single consumer.** A feature is promoted into
  the engine only once two applications need it.
- **Specialize, don't fork.** `RenderWorld` is the one base class; each
  application derives it to say where its entities come from (the Client
  keeps its own list, the Server reads live state, the Editor has none).

## Stack

C++20 · Qt 6 (Core, Network, Qml, Quick, WebSockets) · jsoncpp · GoogleTest
(unit tests for the world model, camera, factories, catalogs and DTOs).

Route and message contract: [`API.md`](API.md).
