# MMORPGClient — technical reference

Game client. Connects to the Server over REST (`Engine::HttpClient`), logs
in, syncs the world data (`/data/manifest` + `/data/{path}`), manages the
account's characters, opens a WebSocket (`Engine::WebSocketClient`) for the
selected character and opens the game screen.

## Screen flow

`ClientWindow.qml` is a `StackView`:

```
LoginPage ──► SyncPage ──► AccountPage ──► GamePage
   ▲                            │
   └──────── logout ────────────┘
```

| Page          | Control              | Responsibility                                          |
| ------------- | -------------------- | ------------------------------------------------------- |
| `LoginPage`   | `LoginPageControl`   | server address (`ServerPanel`/`ServerStatus`), credentials (`AccountPanel`), `/login` and `/sign` |
| `SyncPage`    | `SyncPageControl`    | downloads `/data/manifest`, hash-compares against the local per-server cache, downloads only what's missing/changed via `/data/{path}`, then `DataManager::reload()`s from it |
| `AccountPage` | `AccountPageControl` | master-detail: character list (from `AccountManager.characters`, populated by `/login`/`/sign`) + selected-character actions (create via `/create`, delete via `/remove` with confirm step); "Enter World" passes the selected `idCharacter` to `GamePage` |
| `GamePage`    | `GamePageControl`    | loads the world and renders it through `Viewport`; opens `/ws/character?character=<id>`, drives `activeFloor`/camera/entity from the server's state messages, and sends movement (`move(dx, dy)`) |

## Structure

```
Client/
├── main.cpp, RegisterClientTypes.*   registers types → QML module "MMORPGClientComponents"
├── Manager/
│   ├── ServerManager     address + ConnectionState; get/post (authenticated or not); dataDirectory() (per-server storage path)
│   └── AccountManager    idAccount, username, sessionId, characters (QML singleton, "MMORPGClientManagers")
├── Renderer/
│   └── ClientRenderWorld   Engine::RenderWorld specialization backed by a WorldModel; also holds the client's own entity (setEntity/removeEntity)
└── Application/
    ├── ClientWindow.qml
    ├── Login/    LoginPage.qml, LoginPageControl, Component/{AccountPanel,ServerPanel,ServerStatus}.qml
    ├── Sync/     SyncPage.qml, SyncPageControl
    ├── Account/  AccountPage.qml, AccountPageControl
    └── Game/     GamePage.qml, GamePageControl
```

## Decisions and things to watch

- **The server is the authority** on position: WASD sends
  `{"type":"move","dx","dy"}` over the same WebSocket used for world entry;
  the Client only moves the entity/camera once the server replies with the
  updated (or unchanged, if blocked) `EntityStateDTO`. No local prediction.
- **Active floor**: `viewport.activeFloor` (the same property used by Editor
  and Server) must be driven by the authoritative Z of the player's entity
  received from the server, not by a UI click. No Engine change is needed —
  the property is agnostic about who sets it.
- **Partial loading**: once the Client loads only the chunks/floors near the
  player, `WorldModel::floors()` will mean "floors loaded on the client",
  not the full world list. Do not reuse Editor/Server code that assumes
  otherwise.
- **Per-server data storage**: the world/catalog data the Client syncs lives
  under the OS app-data folder (`QStandardPaths::AppDataLocation`, e.g.
  `%LOCALAPPDATA%/MMORPG/Client/` on Windows), in a `ClientData/<host>_<port>/`
  subfolder keyed by the connected server (`ServerManager::dataDirectory()`),
  so syncing against multiple servers doesn't mix their data. `SyncPageControl`
  mirrors data there; `DataManager`/`GamePageControl` in the Client always
  point there, never at the repo's `Data/` folder (dev-authoring only, see
  [`Data.md`](Data.md)). Nothing initializes `DataManager` at Client startup
  anymore — the first `SyncPage::sync()` does it, so `GamePage` is only
  reachable after a sync.
- **Character list comes from the login response, not a dedicated endpoint**:
  `AccountDTO.characters` (already returned by `/login`/`/sign`) is stored in
  `AccountManager` and mutated locally (`addCharacter`/`removeCharacter`) as
  `AccountPageControl` calls `/create`/`/remove`. There is no `GET /characters`
  — see [`API.md`](API.md).
- **No character appearance/texture concept yet**: `EntityRenderer` draws a
  placeholder (a generated circle) for every entity, since `CharacterModel`/
  `EntityStateDTO` don't carry any sprite reference.
