# MMORPGEditor

The world-authoring tool for MyMMO. A desktop Qt Quick application that
opens the active map, lets you paint it floor by floor and writes the
result straight back to the JSON files the Server loads.

## What you can do

- **Paint tiles and objects** from side palettes onto the map, one click
  per cell, with a live preview through the same renderer the game uses.
  Picking "Empty"/"None" clears a cell.
- **Select** any cell to inspect what's there.
- **Work in layers** — switch between floors (Z levels), add a floor above
  or below, remove one. Empty cells on the current floor show the floor
  beneath, so multi-level areas are easy to line up.
- **Register new tiles and objects** in the catalog: give them a name, a
  texture, tags, whether they can be walked on, and (for objects) a
  footprint. Types are assigned automatically.
- **Configure the map** — choose which map folder is active and resize it.
- **Save** only when you decide to; nothing touches disk until then.

## Under the hood

- Built on the engine's `Viewport`; the Editor adds selection state
  machines, palette models with icon providers, and a `WorldControl`
  backend that mutates the `WorldModel` and persists it.
- Two QML modules: `MMORPGEditorComponents` (pages and components) and
  `MMORPGEditorControls` (C++ backends). Screens are QML; logic is C++.
- Theme, buttons, inputs and icons come from the shared `MMORPGUI` module,
  so the Editor looks like the rest of the project without duplicating UI
  code.
- Selection controls are unit-tested with GoogleTest.

## Stack

C++20 · Qt 6 (Quick, QuickControls2, QuickDialogs2) · MMORPGEngine ·
MMORPGUI.
