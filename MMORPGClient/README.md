# MMORPGClient

The game client — this is the app players run to actually play.

## Running it

1. Build the project (see the [main README](../README.md#getting-started)
   if you haven't yet) and run the `MMORPGClient` executable.
2. On the first screen, type the server's address (ask whoever is running
   the server for it — e.g. `127.0.0.1:8080` for a server on your own
   machine) and log in or create an account.
3. The client will sync the world data from the server — this can take a
   moment the first time, and is much faster afterwards since it only
   re-downloads what changed.
4. Pick a character (or create one), then **Enter World**.
5. Move with **W A S D**.

## Something wrong?

- **Can't connect**: double-check the server address and that the server is
  actually running and reachable from your machine.
- **Login fails**: confirm the username/password, or use "create account"
  if you don't have one yet.
- **Stuck on sync**: this needs a working connection to the server for the
  whole download — if it fails partway, just retry.

More about it: [`Doc/MMORPGClient.md`](../Doc/MMORPGClient.md).
