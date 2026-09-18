# MMORPGEditor

The world editor — used to build and edit the map (tiles, objects, floors)
that the Server and Client load.

## Running it

Build the project (see the [main README](../README.md#getting-started) if
you haven't yet) and run the `MMORPGEditor` executable from its own build
folder. It opens the currently active map (set in `Data/Config.json`).

## Basic usage

- **Select** / **Paint** modes switch what clicking does (top toolbar).
- Pick a tile or object from the palette on the side, then paint it onto
  the map. Pick "None" in the object palette to erase an object from a
  cell.
- Switch floors with the floor selector.
- Save your changes — nothing is written to disk until you do.
- Register a new tile (with its own texture) from the tile catalog screen.

More about it: [`Doc/MMORPGEditor.md`](../Doc/MMORPGEditor.md).
