# MMORPGServer

The game server — this is what the Client(s) connect to. It hosts the world,
handles accounts/characters and keeps track of where everyone is.

## Running it

1. Build the project (see the [main README](../README.md#getting-started)
   if you haven't yet).
2. **Create the database** (one-time step, before the first run): the
   server expects a SQLite file at `Database/ServerDatabase`, next to the
   `Database/` folder in the repo root. Create it by running the schema in
   [`Database/Schema.sql`](../Database/Schema.sql) against a new file at
   that path (any SQLite tool works, e.g. `sqlite3 Database/ServerDatabase
   < Database/Schema.sql`). This isn't automatic yet — if you skip it, the
   server will crash as soon as someone tries to enter the world.
3. Run the `MMORPGServer` executable from its own build folder (it looks
   for `Data/` and `Database/` relative to where it's running from).
4. It listens on port **8080**. Give players your machine's address (e.g.
   `127.0.0.1:8080` for the same machine, or your local network IP for
   others on the same network) so they can connect from the Client.
5. A debug window opens showing the map and every connected character —
   this is just for keeping an eye on things, no interaction needed.

## Something wrong?

- **Crashes right when a character enters the world**: almost always the
  database step above — the schema is still evolving, so if you'd already
  created the database before, you may be missing a newer table. Check
  [`Database/Schema.sql`](../Database/Schema.sql) for what should exist.
- **Client can't connect**: check the address/port you gave them, and that
  nothing (firewall, etc.) is blocking port 8080.
- **Nobody outside your machine can connect**: they need your local network
  or public IP, not `127.0.0.1` — that only works from the same machine.

Route reference for developers: [`Doc/API.md`](../Doc/API.md).
Technical details: [`Doc/MMORPGServer.md`](../Doc/MMORPGServer.md).
