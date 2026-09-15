# Server API

Consolidated documentation for the endpoints/controllers currently available on the server.
The routes below reflect the paths registered in Drogon controllers.

## Controller Overview

### AuthController
File: Server/Network/Rest/AuthController.h

Routes:
- POST, OPTIONS /login
- POST, OPTIONS /logout
- POST, OPTIONS /sign

Notes:
- /logout is protected by Engine::AuthFilter.

### CharacterController
File: Server/Network/Rest/CharacterController.h

Routes:
- POST, OPTIONS /create
- POST, OPTIONS /remove

Notes:
- There is no standalone list endpoint: the account's characters are
  returned embedded in the `/login`/`/sign` response (`AccountDTO.characters`).

### DataController
File: Server/Network/Rest/DataController.h

Routes:
- GET, OPTIONS /data/manifest
- GET, OPTIONS /data/{path}

Notes:
- The manifest lists every file under `Config.json` + the active map folder
  (see [`Data.md`](Data.md)), with an MD5 hash per file
  (`Engine::ManifestFactory`). The Client compares it against its local
  cache and only downloads files whose hash differs or that are missing.
- `path` is the manifest-relative path (e.g. `TestMap/Textures/Tiles/Grass/Grass.png`).
  The server resolves it under the active `Data/` root and rejects anything
  that escapes it.

### StatusController
File: Server/Network/Rest/StatusController.h

Routes:
- GET, OPTIONS /status

### CharacterWebSocket
File: Server/Network/WebSocket/CharacterWebSocket.h

Route:
- WS /ws/character

## Current REST Contract

### POST /login
Controller: AuthController::login

Request body:
```json
{
  "username": "luiz",
  "password": "123"
}
```

Response 200:
```json
{
  "idAccount": 1,
  "username": "luiz",
  "sessionId": "session-uuid",
  "characters": [
    { "idCharacter": 10, "name": "Knight" }
  ]
}
```

Errors:
- 400: { "error": "No JSON" }
- 400: { "error": "Invalid JSON" }
- 401: { "error": "Invalid credentials" }

### POST /sign
Controller: AuthController::sign

Request body:
```json
{
  "username": "luiz",
  "password": "123"
}
```

Response 201:
```json
{
  "idAccount": 1,
  "username": "luiz",
  "sessionId": "session-uuid",
  "characters": []
}
```

Errors:
- 400: { "error": "No JSON" }
- 400: { "error": "Invalid JSON" }
- 400: { "error": "Username or password empty" }
- 409: { "error": "Failed to create account" }
- 500: { "error": "Failed to create account" }
- 500: { "error": "Account created but not found" }

### POST /logout
Controller: AuthController::logout

Header:
- Authorization: X-Session <sessionID>

Response 200:
```json
{
  "message": "Logout successful"
}
```

Errors:
- 401: no standard body (invalid/missing token)
- 500: { "error": "Failed to destroy session" }

### POST /create
Controller: CharacterController::create

Header:
- Authorization: X-Session <sessionID>

Request body:
```json
{
  "name": "Knight"
}
```

Response 201:
```json
{
  "message": "Character created",
  "idCharacter": 10
}
```

Errors:
- 400: {}
- 401: no standard body
- 500: no standard body

### POST /remove
Controller: CharacterController::remove

Header:
- Authorization: X-Session <sessionID>

Request body:
```json
{
  "characterId": 10
}
```

Response 200:
```json
{
  "message": "Character removed"
}
```

Errors:
- 400: {}
- 401: no standard body
- 403: no standard body
- 500: no standard body

### GET /data/manifest
Controller: DataController::downloadManifest

Header:
- Authorization: X-Session <sessionID>

Response 200:
```json
{
  "data": [
    { "path": "Config.json", "hash": "d41d8cd98f00b204e9800998ecf8427e" },
    { "path": "TestMap/Map.json", "hash": "..." },
    { "path": "TestMap/Textures/Tiles/Grass/Grass.png", "hash": "..." }
  ]
}
```

Errors:
- 401: no standard body

### GET /data/{path}
Controller: DataController::downloadData

Header:
- Authorization: X-Session <sessionID>

Response 200:
- Raw file bytes, content type inferred from the extension.

Errors:
- 401: no standard body
- 404: path not found or outside the `Data/` root

### GET /status
Controller: StatusController::status

Response 200:
```json
{
  "message": "Server Active"
}
```

## Current WebSocket Contract

### WS /ws/character
Controller: CharacterWebSocket

Authentication:
- Authorization header: X-Session <sessionID>
- or sid query param

Required parameters:
- character (numeric query param)

On a successful connection, the server validates that `character` belongs to
the account behind the session (`CharacterRepository::findByIdAccountAndIdCharacter`),
loads the character's persisted position (or a placeholder spawn — map
center, lowest loaded floor — if none exists yet), registers the character
in `WorldManager`, and sends one welcome message with that state
(`Engine::EntityStateDTO`):
```json
{
  "idCharacter": 10,
  "x": 16,
  "y": 16,
  "z": 0,
  "worldName": "TestMap"
}
```

Error messages sent by server (connection is closed right after):
```json
{"error":"missing_context"}
```
```json
{"error":"missing_session"}
```
```json
{"error":"missing_character"}
```
```json
{"error":"invalid_session"}
```
```json
{"error":"invalid_character"}
```

After the handshake, the client can send movement requests on the same
connection:
```json
{"type":"move","dx":0,"dy":-1}
```
`dx`/`dy` must each be `-1`, `0` or `1`. The server validates world bounds
and the destination tile's `isWalkable`, updates the character's position
if the move is valid, and always replies with the authoritative state
(unchanged position if the move was blocked):
```json
{
  "idCharacter": 10,
  "x": 16,
  "y": 15,
  "z": 0,
  "worldName": "TestMap"
}
```
