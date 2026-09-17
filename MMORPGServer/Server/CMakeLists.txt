set(PROJECT_SOURCES
    RegisterServerTypes.cpp
    RegisterServerTypes.h
    Application/Server/ServerPageControl.cpp
    Application/Server/ServerPageControl.h
    Database/Database.cpp
    Database/Database.h
    Database/Query.cpp
    Database/Query.h
    Database/QueryLoader.cpp
    Database/QueryLoader.h
    Event/EventBus.cpp
    Event/EventBus.h
    Event/WorldEvent.cpp
    Event/WorldEvent.h
    Event/WorldEventType.h
    Manager/ChunkCoordinate.cpp
    Manager/ChunkCoordinate.h
    Manager/EntityBroadcaster.cpp
    Manager/EntityBroadcaster.h
    Manager/NetworkManager.cpp
    Manager/NetworkManager.h
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
    Network/Rest/DataController.cpp
    Network/Rest/DataController.h
    Network/Rest/StatusController.cpp
    Network/Rest/StatusController.h
    Network/WebSocket/CharacterConnectionContext.cpp
    Network/WebSocket/CharacterConnectionContext.h
    Network/WebSocket/CharacterConnectionRegistry.cpp
    Network/WebSocket/CharacterConnectionRegistry.h
    Network/WebSocket/CharacterWebSocket.cpp
    Network/WebSocket/CharacterWebSocket.h
    Network/WebSocket/MessageReceiver.cpp
    Network/WebSocket/MessageReceiver.h
    Renderer/ServerRenderWorld.cpp
    Renderer/ServerRenderWorld.h
    Repository/AccountRepository.cpp
    Repository/AccountRepository.h
    Repository/CharacterPositionRepository.cpp
    Repository/CharacterPositionRepository.h
    Repository/CharacterRepository.cpp
    Repository/CharacterRepository.h
    Repository/CharacterVitalsRepository.cpp
    Repository/CharacterVitalsRepository.h
    Repository/Repository.cpp
    Repository/Repository.h
    Runtime/CharacterRuntime.cpp
    Runtime/CharacterRuntime.h
    Runtime/CharacterSystem.cpp
    Runtime/CharacterSystem.h
    Runtime/CreatureRuntime.cpp
    Runtime/CreatureRuntime.h
    Runtime/WorldRuntime.cpp
    Runtime/WorldRuntime.h)

qt_add_executable(${PROJECT_NAME} main.cpp ${PROJECT_SOURCES})

qt_add_qml_module(
  ${PROJECT_NAME}
  URI
  MMORPGServerComponents
  VERSION
  1.0
  QML_FILES
  Main.qml
  Application/ServerWindow.qml
  Application/Server/ServerPage.qml
  Application/Server/ServerSidebar.qml
  Application/Server/FloorSelector.qml
  Application/Server/CharacterListPanel.qml
  Application/Server/CharacterCard.qml)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Drogon::Drogon
  PRIVATE Qt6::QuickControls2
  PRIVATE MMORPG::MMORPGEngine
  PRIVATE MMORPG::MMORPGUI)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::sqlite3::sqlite3)
endif()
