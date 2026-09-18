# MMORPGEditor — technical reference

World editor. Opens the active map in `Data/`, lets you edit tiles, objects
and floors, and saves back to the JSON files. Also registers new tiles in
the catalog.

## What it does today

- Loads the world (`DataManager` → `WorldModel` → `EditorRenderWorld`) and
  centers the camera.
- Mutually exclusive tile/object selection; **Select** and **Paint** modes
  (`ToolModeBar`).
- Paints tiles and objects from the palettes; removes an object by picking
  "None" (`type 0`).
- Switches the active floor (`FloorSelector`).
- Saves `Floors/*.json` (`WorldControl::saveWorld()`).
- Tile registration (name, texture, tags, `isWalkable`, auto-assigned
  `Type`) written to `Tile.json`.

## Structure

```
Editor/
├── main.cpp                 registers Engine and Editor types, loads Main.qml
├── RegisterEditorTypes.*    qmlRegisterType → QML module "MMORPGEditorControls"
├── Renderer/
│   └── EditorRenderWorld    Engine::RenderWorld specialization for the Editor
└── Application/
    ├── EditorWindow.qml     StackView + NavigationBar (World / Tiles / Objects)
    ├── Navigation/          NavigationBar.qml
    ├── World/               main editing page
    │   ├── WorldPage.qml, EditorSidebar.qml
    │   ├── BrushMode.qml                      (QML singleton)
    │   ├── WorldControl.*                     world load/save, paintTile/paintObject
    │   ├── Tool/ToolSelector.qml              floating bar over the Viewport (ToolModeBar + FloorSelector)
    │   ├── Tool/ToolMode.qml, ToolModeBar.qml (Select/Paint modes)
    │   ├── Floor/FloorSelector.qml            Z0/Z1/... buttons
    │   ├── Tile/                              TilePalette.qml, TilePaletteModel, TileSelectionControl, TileIconProvider
    │   └── Object/                            ObjectPalette.qml, ObjectPaletteModel, ObjectSelectionControl, ObjectIconProvider
    ├── Tiles/               TileCreationPage.qml + TileCreationControl (catalog registration)
    └── Objects/             ObjectCreationPage.qml + ObjectCreationControl (catalog registration)
```

Two QML modules: `MMORPGEditorComponents` (`.qml` files) and
`MMORPGEditorControls` (C++ classes registered in `RegisterEditorTypes`).

## Selection/editing flow

```
Mouse → Viewport → Camera::screenToWorld() → world position → tile/object
      → *SelectionControl → visual feedback (highlight)
Paint: palette (Tile/ObjectPaletteModel) → WorldControl::paintTile/paintObject
      → WorldTileModel/WorldObjectModel → EditorRenderWorld → Viewport
```

## Decisions

- The `Viewport`'s `activeFloor` is set directly by the UI
  (`FloorSelector`). Server and Client use the same mechanism; only the
  "driver" changes.
- `FloorSelector.qml` is Editor-specific. Promote it to `MMORPGUI` only once
  the Server has a real use of its own to compare against.
- Tile tags are only for organizing/grouping the palette — they have no
  effect on the world or the render.
- Objects have no orientation in the current model; object
  position/orientation waits until the concept exists in the Engine.
- `EditorRenderWorld::entities()` returns an empty list — the Editor has no
  entity concept yet. It's the natural place to show creature spawn markers
  later.
