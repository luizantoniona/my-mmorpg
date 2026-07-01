set(APP_SOURCES
  Database/Database.cpp
  Database/Query.cpp
  Database/QueryLoader.cpp
  Manager/WorldManager.cpp
  Network/NetworkServer.cpp
  Network/NetworkSession.cpp
  Network/Filter/AuthFilter.cpp
  Network/Rest/AuthController.cpp
  Network/Rest/CharacterController.cpp
  Network/Rest/StatusController.cpp
  Network/WebSocket/CharacterWebSocket.cpp
  Network/WebSocket/MessageReceiver.cpp
  Repository/AccountRepository.cpp
  Repository/CharacterRepository.cpp
  Repository/Repository.cpp
)

set(APP_HEADERS
  Database/Database.h
  Database/Query.h
  Database/QueryLoader.h
  Manager/WorldManager.h
  Network/NetworkServer.h
  Network/NetworkSession.h
  Network/Filter/AuthFilter.h
  Network/Rest/AuthController.h
  Network/Rest/CharacterController.h
  Network/Rest/StatusController.h
  Network/WebSocket/CharacterWebSocket.h
  Network/WebSocket/MessageReceiver.h
  Repository/AccountRepository.h
  Repository/CharacterRepository.h
  Repository/Repository.h
)

add_executable(${PROJECT_NAME} main.cpp ${APP_SOURCES} ${APP_HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)
if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::sqlite3::sqlite3)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE Engine::EngineLib)
