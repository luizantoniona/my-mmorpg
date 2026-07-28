set(PROJECT_SOURCES
    Database/Database.cpp
    Database/Database.h
    Database/Query.cpp
    Database/Query.h
    Database/QueryLoader.cpp
    Database/QueryLoader.h
    Manager/WorldManager.cpp
    Manager/WorldManager.h
    Network/NetworkServer.cpp
    Network/NetworkServer.h
    Network/NetworkSession.cpp
    Network/NetworkSession.h
    Network/Filter/AuthFilter.cpp
    Network/Filter/AuthFilter.h
    Network/Rest/AuthController.cpp
    Network/Rest/AuthController.h
    Network/Rest/CharacterController.cpp
    Network/Rest/CharacterController.h
    Network/Rest/StatusController.cpp
    Network/Rest/StatusController.h
    Network/WebSocket/CharacterWebSocket.cpp
    Network/WebSocket/CharacterWebSocket.h
    Network/WebSocket/MessageReceiver.cpp
    Network/WebSocket/MessageReceiver.h
    Repository/AccountRepository.cpp
    Repository/AccountRepository.h
    Repository/CharacterRepository.cpp
    Repository/CharacterRepository.h
    Repository/Repository.cpp
    Repository/Repository.h)

add_executable(${PROJECT_NAME} main.cpp ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)
if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::sqlite3::sqlite3)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE MMORPG::MMORPGEngine)
