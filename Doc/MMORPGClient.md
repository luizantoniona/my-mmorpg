# MMORPGClient

The player-facing application of MyMMO. A Qt Quick desktop client that
logs into a server, syncs the world, lets you pick a character and drops
you into the map.

## What it does

- **Connect and log in** — point it at a server, check it's alive, sign up
  or sign in.
- **Sync the world** — downloads the server's data manifest, compares
  hashes with the local per-server cache and fetches only new or changed
  files. A second launch against the same server is nearly instant.
- **Manage characters** — master-detail list, create and delete with
  confirmation, then *Enter World*.
- **Play** — the map renders through the engine `Viewport`; WASD sends
  movement requests and the character moves when the server confirms.
  Other players nearby appear and disappear in real time. A HUD shows your
  name and health / mana / stamina bars, plus an action bar ready for
  inventory, skills and settings.

## Architecture highlights

- **Page ↔ Control.** Every QML page has a C++ `*PageControl` backend
  (`Login`, `Sync`, `Account`, `Game`). QML orchestrates; logic and
  networking live in C++.
- **Server-authoritative movement.** The client never predicts. It sends
  `CHARACTER_MOVE` and applies whatever `OWN_CHARACTER` state comes back —
  including "you didn't move" when a step is blocked.
- **Typed messages.** Every WebSocket message carries a `type`; the client
  routes on it and knows from the type alone whether a message is about
  its own character or someone else.
- **Per-server data cache.** Synced data lives under the OS app-data
  folder, keyed by host and port, so switching servers never mixes worlds.
- **Shared UI kit.** Panels, inputs, buttons and theme tokens come from
  `MMORPGUI`; the client only adds game-specific components (HUD, stat
  bars, action bar).

## Stack

C++20 · Qt 6 (Quick, QuickControls2, Network, WebSockets) · MMORPGEngine ·
MMORPGUI · GoogleTest (render-world tests).

Route and message contract: [`API.md`](API.md).
