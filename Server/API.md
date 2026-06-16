# Server API

Consolidated documentation for the endpoints/controllers currently available on the server.
The routes below reflect the paths registered in Drogon controllers.

## Controller Overview

### AuthController
File: Server/Engine/Network/Rest/AuthController.h

Routes:
- POST, OPTIONS /login
- POST, OPTIONS /logout
- POST, OPTIONS /sign

Notes:
- /logout is protected by Engine::AuthFilter.

### CharacterController
File: Server/Engine/Network/Rest/CharacterController.h

Routes:
- POST, OPTIONS /create
- POST, OPTIONS /remove
- GET, OPTIONS /characters

### StatusController
File: Server/Engine/Network/Rest/StatusController.h

Routes:
- GET, OPTIONS /status

### CharacterWebSocket
File: Server/Engine/Network/WebSocket/CharacterWebSocket.h

Route:
- WS /ws/character

## DTOs sent to the client

### AccountDTO
File: Server/Engine/Account/AccountDTO.h

Fields:
- idAccount (int)
- username (string)

Example:
```json
{
  "idAccount": 1,
  "username": "luiz"
}
```

### CharacterDTO
File: Server/Engine/Character/CharacterDTO.h

Fields:
- idCharacter (int)
- idAccount (int)
- name (string)

Example:
```json
{
  "idCharacter": 10,
  "idAccount": 1,
  "name": "Knight"
}
```

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
  "sessionID": "session-uuid",
  "message": "Login successful"
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
  "sessionID": "session-uuid",
  "message": "Account created successfully"
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

### GET /characters
Controller: CharacterController::list

Header:
- Authorization: X-Session <sessionID>

Response 200 (current):
```json
{
  "characters": []
}
```

Note:
- If there are characters, the current implementation builds each item as an empty string, so the payload may be ["", ""].

Errors:
- 401: no standard body

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

Error messages sent by server:
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

## Compatibility Notes

- The schema and DTOs use account/idAccount.
- Server controllers and session internals are now standardized on account/idAccount naming.
