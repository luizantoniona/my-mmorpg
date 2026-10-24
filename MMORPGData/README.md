# MMORPGData

The game content: everything the Server loads and the Client downloads. It is
plain JSON plus textures, so a world can be written by hand or built with the
[Editor](../MMORPGEditor/README.md). [`ExampleMap/`](ExampleMap/) is a working
reference for every format described here.

Do not confuse it with `MMORPGEngine/Data/`, which is the *code* that reads
these files.

## Layout

```
MMORPGData/
├── Config.json                 which map is active + server settings
└── <MapFolder>/                e.g. ExampleMap/
    ├── Map.json                size, spawn, floors, catalog file names
    ├── Tile.json               tile catalog
    ├── Object.json             object catalog
    ├── Items.json              index of item types
    ├── Items/<TYPE>.json       one file per item type (+ its items and skill tree)
    ├── Creatures.json          index of creature types
    ├── Creatures/<TYPE>/       <TYPE>.json + the sprite sheets of the creature
    ├── Floors/<z>.json         one file per floor
    └── Textures/
        ├── Tiles/<Type>.png    (.gif / .webp when animated)
        ├── Objects/<Type>.png
        └── Items/<Id>.png
```

General rules:

- Keys written as **text ids** (`IRON_SWORD`, `GOBLIN`, `SWORD`) must match
  `^[A-Z0-9_]+$`.
- Tiles and objects are identified by a **numeric `Type`**; `0` always means
  "empty" and has no catalog entry.
- An entry with a **wrongly typed field** is skipped with a warning and the
  rest of the file still loads. The headers (`Config.json`, `Map.json`, the
  `Z` of a floor) fail loudly instead.
- A world is `32 × 32` tiles per chunk and `32 px` per tile.
- `TextureFile` is a path **without extension**, relative to the map folder
  (`Textures/Tiles/12`). By convention the file is named after the registered
  `Type` / `Id`.

## `Config.json`

```json
{
    "ActiveFolder": "ExampleMap",
    "Server": { "TickRate": 30 }
}
```

| Field | Notes |
| ----- | ----- |
| `ActiveFolder` | Map folder loaded by the Server and the Editor. |
| `Server.TickRate` | Simulation ticks per second (positive integer). |

## `Map.json`

```json
{
    "Name": "Example Map",
    "Width": 96,
    "Height": 96,
    "SpawnX": 47, "SpawnY": 47, "SpawnZ": 0,
    "Catalogs": {
        "Tiles": "Tile.json",
        "Objects": "Object.json",
        "Items": "Items.json",
        "Creatures": "Creatures.json"
    },
    "Floors": [ "0.json" ]
}
```

`Name`, `Width`, `Height`, `Catalogs` and `Floors` are required. The spawn
defaults to the center of the map at `Z = 0`. `Floors` lists the files under
`Floors/` to load.

## `Tile.json`

```json
{
    "Tiles": [
        {
            "Type": 1,
            "Name": "Grass",
            "Tags": [ "grass" ],
            "TextureFile": "Textures/Tiles/1",
            "IsWalkable": true,
            "IsAnimated": false,
            "FrameDurationMs": 100
        }
    ]
}
```

- `Type`, `Name`, `TextureFile` are required. `Name` is display text only and
  may repeat. `Tags` only organize the Editor palette.
- `IsWalkable` (default `true`) — whether characters and creatures can step
  on it. Write it explicitly when editing by hand.
- `IsAnimated: true` loads `<TextureFile>.gif` or `.webp` instead of `.png`;
  `FrameDurationMs` defaults to `100`.

## `Object.json`

```json
{
    "Objects": [
        {
            "Type": 5,
            "Name": "Oak",
            "TextureFile": "Textures/Objects/5",
            "Footprint": { "Width": 2, "Height": 2 },
            "IsWalkable": false,
            "Interaction": "CONTAINER",
            "ContainerCapacity": 20
        }
    ]
}
```

- Objects sit on top of tiles. `Footprint` (default `1 × 1`) is anchored at
  the **bottom-left** cell.
- `IsWalkable` (default `true`): when `false` the whole footprint blocks
  movement.
- `Interaction` is optional; the only value today is `CONTAINER`, with
  `ContainerCapacity` slots.
- `IsAnimated` / `FrameDurationMs` work as in tiles.

## `Floors/<z>.json`

```json
{
    "Z": 0,
    "Tiles": [ [ 1, 1, 0 ], [ 1, 2, 0 ] ],
    "Overlays": [ { "X": 3, "Y": 4, "Type": 12 } ],
    "Objects": [ { "X": 10, "Y": 7, "Type": 5 } ],
    "SpawnAreas": [
        {
            "X": 20, "Y": 20, "Width": 8, "Height": 8,
            "RespawnSeconds": 30.0,
            "Creatures": [ { "Type": "GOBLIN", "Quantity": 3 } ]
        }
    ]
}
```

- `Tiles` is a `Height × Width` matrix of tile `Type`s (`0` = empty, which
  shows the floor below). `Overlays` and `Objects` are sparse lists.
- Every `Type` must exist in its catalog, otherwise the entry is skipped.
- `SpawnAreas` need a positive `RespawnSeconds`; areas without it are
  skipped. Each creature entry names a creature `Type` (text id) and a
  `Quantity`.
- One file per floor; negative `Z` is below ground.

## Items

`Items.json` is only an index:

```json
{ "ItemTypes": [ "SWORD", "SHIELD", "HELMET" ] }
```

Each listed type has its own file, `Items/<TYPE>.json`. An **item type** is
the class of an item (what it is worn as, how many hands, which skill tree it
unlocks); an **item** is a concrete entry of that type.

```json
{
    "Name": "Sword",
    "Category": "WEAPON",
    "Slot": "HAND",
    "HandRequirement": "ONE_HANDED",
    "DisabledActions": [ "DODGE" ],
    "SkillTree": [
        { "Type": 1, "Name": "Sword Mastery", "ProficiencyLevel": 1, "Prerequisites": [] },
        { "Type": 2, "Name": "Whirlwind Strike", "ProficiencyLevel": 5, "Prerequisites": [ 1 ] }
    ],
    "Items": [
        {
            "Id": "IRON_SWORD",
            "Name": "Iron Sword",
            "Damage": 10,
            "TextureFile": "Textures/Items/IRON_SWORD",
            "Attack": { "CastSeconds": 0.6, "HitMode": "ALL", "Shape": [ "X", "@" ] }
        }
    ]
}
```

### Item type fields

| Field | Values / notes |
| ----- | -------------- |
| `Name`, `Category`, `Slot` | Required. |
| `Category` | `WEAPON` (grants `ATTACK`), `SHIELD` (grants `BLOCK`), `OFFHAND`, `ARMOR`. |
| `Slot` | `HAND`, `HEAD`, `CHEST`, `LEGS`, `FEET`, `GLOVE`, `RING`, `NECK`. |
| `HandRequirement` | `ONE_HANDED` or `TWO_HANDED`; for hand items. |
| `DisabledActions` | Actions the character **loses** while it is equipped: `ATTACK`, `DODGE`, `BLOCK`. A shield disabling `DODGE` is the usual case. |
| `SkillTree` | Optional list of skill nodes: `Type` (numeric id inside the tree), `Name`, `ProficiencyLevel`, `Prerequisites` (list of `Type`s). |
| `Items` | Optional list of concrete items. |

An item type with an unknown `Category` or `Slot` is skipped entirely.

### Item fields

| Field | Notes |
| ----- | ----- |
| `Id`, `Name` | Required. `Id` is a text id, unique across **all** types. |
| `Damage` | Base damage of the weapon. Without it, bare hands do `5`. |
| `Attack` | Attack format of this item, see below. Without it the item uses a basic attack: one cell in front, hits the first creature. |
| `TextureFile` | Inventory / ground icon, e.g. `Textures/Items/IRON_SWORD`. |
| `IsAnimated`, `FrameDurationMs` | As in tiles. |

An invalid `Attack` skips only that item.

### `Attack`

`CastSeconds`, `HitMode` and `Steps` are all **required**: an attack missing
any of them is invalid. The same format is meant for every attacker (items
today; creatures, NPCs and skills later). An attack is **always a list of
`Steps`**, even when it has just one. Each
step is a *strike*: a set of cells hit at the same moment. Several steps give
combos, projectiles and expanding waves.

A step's `Shape` is a small grid written **facing up**. `@` is the character
(exactly one), `X` is a cell that gets hit, and a space or `.` is empty. Rows
read top to bottom, so the cells above the `@` are in front of the character.
Every row of a step should have the same width. The shape always turns to the
direction the character attacks in, so a symmetric shape (a ring) simply hits
the same cells whichever way it is aimed.

A spear that hits the first creature in two cells:

```json
"Attack": {
    "CastSeconds": 0.8,
    "HitMode": "FIRST",
    "Steps": [
        {
            "Delay": 0.0,
            "Damage": 1.0,
            "Shape": [ "X", "X", "@" ]
        }
    ]
}
```

Several steps with their own `Delay` make a wave that expands — here two
rings, the second one 0.3 s after the first:

```json
"Attack": {
    "CastSeconds": 1.5,
    "HitMode": "ALL",
    "Steps": [
        { "Delay": 0.0, "Damage": 1.0, "Shape": [ ".....", ".XXX.", ".X@X.", ".XXX.", "....." ] },
        { "Delay": 0.3, "Damage": 1.0, "Shape": [ "XXXXX", "X...X", "X.@.X", "X...X", "XXXXX" ] }
    ]
}
```

There is no repeat field: to hit the same cells several times, write one step
per hit (here three weak hits, 0.3 s apart):

```json
"Steps": [
    { "Delay": 0.0, "Damage": 0.4, "Shape": [ "X", "@" ] },
    { "Delay": 0.3, "Damage": 0.4, "Shape": [ "X", "@" ] },
    { "Delay": 0.6, "Damage": 0.4, "Shape": [ "X", "@" ] }
]
```

| Field | Notes |
| ----- | ----- |
| `CastSeconds` | **Required.** Time before the first hit, must be `> 0`. |
| `HitMode` | **Required.** Applied to **each strike**: `ALL` hits every creature in the step's cells; `FIRST` stops at the first one, ordered by distance from the `@`. |
| `Steps` | **Required.** Non-empty list; one invalid step invalidates the attack. |
| `Steps[].Delay` | **Required.** Seconds after the cast ends (`≥ 0`). |
| `Steps[].Damage` | **Required.** Multiplier of the item `Damage` (`> 0`). |
| `Steps[].Shape` | **Required.** The grid above: exactly one `@`, at least one `X`. Any other character is invalid. |

## Creatures

`Creatures.json` is an index, like `Items.json`:

```json
{ "Creatures": [ "COW", "ORC" ] }
```

Each key has `Creatures/<TYPE>/<TYPE>.json`:

```json
{
    "Name": "Orc",
    "AggroRadius": 5,
    "AttackRange": 1,
    "Footprint": 1,
    "Vitals": { "MaxHealth": 40 },
    "Appearance": { }
}
```

| Field | Default | Notes |
| ----- | ------- | ----- |
| `Name`, `Vitals.MaxHealth` | required | |
| `AggroRadius` | `0` | Radius, in tiles, in which it notices a character. |
| `AttackRange` | `1` | Tiles. |
| `Footprint` | `1` | Side of the `N × N` square it occupies, anchored at the top-left. |
| `Appearance` | circle placeholder | Sprite animation, below. |

### `Appearance`

```json
"Appearance": {
    "FrameSize": [ 128, 128 ],
    "Clips": {
        "walk": {
            "File": "cow_walk.png",
            "Rows": { "north": 0, "west": 1, "south": 2, "east": 3 },
            "From": 0,
            "Count": 4,
            "FrameDurationMs": 120
        }
    }
}
```

- `Clips` keys are states: `idle`, `walk`, `attack`, `dodge`, `block`, `dead`.
- `File` is a sprite sheet in the creature's folder. `Rows` maps each
  direction (`north`, `south`, `east`, `west`) to a row of the sheet.
- `FrameSize` `[width, height]` can be set on the whole `Appearance` or per
  clip. `Stride` (default = `FrameSize`) is the distance between frames when
  the sheet has padding. `From` (default `0`) is the first column.
- `Count` and `FrameDurationMs` are required and positive.

## Checking your changes

The Server and the Editor log a warning for every skipped entry, so start
either one and read the log after editing by hand. The Engine tests cover
each loader (`MMORPGEngine/Test/`).
