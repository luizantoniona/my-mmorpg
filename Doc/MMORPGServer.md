# MMORPGServer — technical reference

Game server. Starts a REST + WebSocket API (Drogon) on `0.0.0.0:8080`,
persists accounts/characters in SQLite, loads the active world from `Data/`
and opens a Qt debug window that renders the map with the same Engine
`Viewport`. Classes live in `namespace Server`.

Route contract: [`API.md`](API.md).

## Lifecycle

`main.cpp` initializes singletons in this order and finalizes them in
reverse:

```
Database  (Database/ServerDatabase via ../../../Database/)
  → DataManager  (Engine: manifest + catalogs from Data/)
    → WorldManager  (WorldModel of the active map, own thread, tracks connected characters)
      → NetworkManager  (Drogon: listeners, controllers, filters)
        → QML Main (MMORPGServerComponents)
```

Paths are relative to the executable (`../../../Data/`, `../../../Database/`),
so run from the Server's build folder.

## Structure

```
Server/
├── main.cpp, RegisterServerTypes.*      registers types → QML module "MMORPGServerComponents"
├── Database/    Database (SQLite), Query, QueryLoader
├── Repository/  Repository (base), AccountRepository, CharacterRepository, CharacterPositionRepository
├── Manager/     NetworkManager (Drogon), WorldManager (WorldModel + thread, connected characters, characterPositions())
├── Network/
│   ├── NetworkServer, NetworkSession        authenticated sessions (sessionID)
│   ├── Filter/AuthFilter                    protects routes by session
│   ├── Rest/                                AuthController (/login, /sign, /logout)
│   │                                        CharacterController (/create, /remove) — no list route, see API.md
│   │                                        DataController (/data/manifest, /data/{path}) — serves Data/ for the Client to sync
│   │                                        StatusController (/status)
│   └── WebSocket/                           CharacterWebSocket (/ws/character), CharacterConnectionContext, MessageReceiver (dispatches move requests)
├── Renderer/    ServerRenderWorld            Engine::RenderWorld specialization; entities() reads live from WorldManager
└── Application/ ServerWindow.qml, Server/ServerPage.qml   debug window with Viewport
```

## Database

Schema in [`../Database/Schema.sql`](../Database/Schema.sql): `account`,
`character`, `character_inventory`, `character_position`,
`character_vitals`. The `Database/ServerDatabase` file is git-ignored —
create it locally from the schema before starting the server (see the
project [`README.md`](../README.md) for the exact steps). There is no
automatic migration yet (`Database::migrate()` is a stub) — schema changes
must be applied by hand while the tables are still evolving.

## Decisions

- **The server is the authority** over entity position and state; the
  Client sends requests (movement, etc.) and receives the validated state.
- The debug window uses the same `Engine::Viewport` as the Editor, so it
  already honors `activeFloor` and renders every connected character as an
  entity (via `ServerRenderWorld::entities()` → `WorldManager::characterPositions()`,
  read fresh on every repaint — no polling loop needed). It has no UI of its
  own to switch floors yet.
- `ServerRenderWorld` currently exposes only `object`/`tile`/`entities`. For
  the Runtime Debug View (Phase 4) it still needs: `world`/`floors` exposed
  as `Q_PROPERTY` (as `WorldControl` does in the Editor) and its own floor
  selector — without sharing the Editor's `FloorSelector.qml` until there
  are two real uses.
- **`/ws/character` handshake**: validates the session and that the
  character belongs to the account
  (`CharacterRepository::findByIdAccountAndIdCharacter`), loads any
  persisted position (`CharacterPositionRepository`, falling back to a
  placeholder spawn — map center, lowest loaded floor — if none exists),
  registers the character in `WorldManager`, and sends one
  `Engine::EntityStateDTO` with that state.
- **Movement**: `MessageReceiver::receiveMove` validates a single-tile step,
  world bounds and the destination tile's `isWalkable`, updates the
  character's position on success, and always replies with the
  authoritative `EntityStateDTO` (unchanged position if the move was
  blocked). Objects on the destination tile don't block movement yet
  (`ObjectModel` has no `isWalkable`/collision concept). Position is only
  persisted on disconnect (`CharacterWebSocket::handleConnectionClosed`),
  not on every step.
