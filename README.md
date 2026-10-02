# 🧙‍♂️ MyMMO 🧌

A 2D top-down MMORPG written in C++20 / Qt 6 (QML). A monorepo with a shared
engine and three applications: Server, Client and Editor.


## 📦 Organization

| Module | Description |
| --- | --- |
| ⚙️ [`MMORPGEngine`](MMORPGEngine/README.md) | Shared static library: world and entity models, data catalogs, renderer and QML `Viewport` |
| 🎨 [`MMORPGUI`](MMORPGUI/) | QML module `MMORPGUIComponents`: base components and theme singletons |
| 🛠️ [`MMORPGServer`](MMORPGServer/README.md) | Drogon server (REST + WebSocket), SQLite, world debug window |
| 🎮 [`MMORPGClient`](MMORPGClient/README.md) | Game client: login, data sync, account/characters, game |
| 🧩 [`MMORPGEditor`](MMORPGEditor/README.md) | World editor: tiles, objects, floors, catalog |
| 📁 `Data` | Active world: `Config.json` picks the map folder; each map has `Map.json`, tile/object catalogs, one JSON per floor and the textures |
| 🗄️ `Database` | SQLite `Schema.sql` (account, character, inventory, position, vitals) |
| 📚 [`Doc`](Doc/) | A presentation of each module + the [server API](Doc/API.md) contract |

## 🚀 Getting started

### Prerequisites

- CMake ≥ 3.16
- Qt 6 (Core, Network, Qml, Quick, QuickControls2, QuickEffects)
- vcpkg with `VCPKG_ROOT` set — dependencies in [`vcpkg.json`](vcpkg.json):
  `drogon`, `jsoncpp`, `sqlite3`, `gtest`

### Clone and build

```bash
git clone https://github.com/luizantoniona/my-mmorpg.git
cd my-mmorpg
cmake -S . -B build
cmake --build build
```

The root `CMakeLists.txt` picks up the vcpkg toolchain automatically from
`VCPKG_ROOT`. The `BUILD_SERVER`, `BUILD_CLIENT` and `BUILD_EDITOR` options
(all ON by default) let you build only what you need. The project is usually
opened directly in Qt Creator.

### Running

Server, Client and Editor resolve `Data/` and `Database/` relative to the
executable, so run each one from its own build folder. Start with the
Server — see its [README](MMORPGServer/README.md) for the one-time database
setup step — then the Client or Editor.

## 🧑‍🚀 People

This project was envisioned, created and is maintained by:
- **[Luiz Antonio Nicolau Anghinoni](https://github.com/luizantoniona)**

License: [MIT](LICENSE)
