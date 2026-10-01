# MMORPGServer

The authoritative game server for MyMMO. A single C++ process that exposes
a REST API and a WebSocket endpoint (Drogon), persists accounts and
characters in SQLite, runs the live world on its own tick thread and opens
a Qt debug window that renders the map and every connected character.

## What it does

- **Accounts and sessions** — sign up, log in, log out; protected routes
  are gated by a session filter.
- **Characters** — create and remove characters; each one gets a spawn
  position and starting vitals persisted alongside it.
- **World data distribution** — serves the active world folder with an
  MD5 manifest, so clients download only what changed.
- **Live world** — characters enter through `/ws/character`, move one tile
  at a time, and the server validates every step (bounds, walkability)
  before replying with the authoritative state.
- **Proximity broadcast** — the world is indexed by chunk; when something
  changes, only characters in the surrounding 3×3 chunks are told.
- **Debug window** — the same engine `Viewport` as the Editor, in
  read-only mode, with a live list of connected characters (click to
  center the camera, disconnect from the UI). It watches the world as a
  client would, so movement and attacks animate there exactly as they do
  in the game.

## Architecture highlights

- **The server is the authority.** Clients send intents; the server
  validates and pushes state. There is no client-side prediction, which
  keeps the protocol small and the rules in one place.
- **Runtime, systems and events.** `WorldManager` owns lifecycle (thread,
  bootstrap); `WorldRuntime` owns live state (characters, chunk index,
  event bus) and never builds a network message. Per-entity `*Runtime`
  objects host pluggable `*System`s that run every tick. State changes are
  published as events; a single `EntityBroadcaster` turns events into DTOs
  and sends them.
- **One delivery path, two kinds of viewer.** The broadcaster doesn't know
  about sockets: it sends to *observers*. A player's observer wraps a
  WebSocket connection and sees only its surroundings; the debug window
  registers a global observer that receives the same DTOs in-process. So
  the debug window consumes the real protocol instead of reading the
  runtime directly — it inherits the client's rendering for free, and any
  divergence between it and the runtime-backed character list is itself a
  signal that the broadcast is wrong. The same seam is what a spectator
  mode, an admin client, or socket-free tests of the broadcast layer would
  plug into.
- **Mutex discipline.** Runtime methods lock per call; events are published
  only after the lock is released, so subscribers can safely query the
  runtime back.
- **Repository pattern.** All SQLite access goes through `*Repository`
  classes; creating a character creates its derived rows in one place.
- **Testable core.** `WorldRuntime`, the event bus and the chunk index are
  unit-tested without linking Drogon; CI builds and runs them on every
  push.

## Stack

C++20 · Drogon (REST + WebSocket) · SQLite · jsoncpp · Qt 6 (debug
window) · MMORPGEngine · MMORPGUI · GoogleTest.

Route and message contract: [`API.md`](API.md).
