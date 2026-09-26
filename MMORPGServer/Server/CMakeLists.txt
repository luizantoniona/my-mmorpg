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
    Repository/Account/AccountRepository.cpp
    Repository/Account/AccountRepository.h
    Repository/Character/CharacterEquipmentRepository.cpp
    Repository/Character/CharacterEquipmentRepository.h
    Repository/Character/CharacterInventoryRepository.cpp
    Repository/Character/CharacterInventoryRepository.h
    Repository/Character/CharacterPositionRepository.cpp
    Repository/Character/CharacterPositionRepository.h
    Repository/Character/CharacterProficiencyRepository.cpp
    Repository/Character/CharacterProficiencyRepository.h
    Repository/Character/CharacterRepository.cpp
    Repository/Character/CharacterRepository.h
    Repository/Character/CharacterVitalsRepository.cpp
    Repository/Character/CharacterVitalsRepository.h
    Repository/Repository.cpp
    Repository/Repository.h
    Runtime/Character/CharacterRuntime.cpp
    Runtime/Character/CharacterRuntime.h
    Runtime/Character/System/CharacterCombatSystem.cpp
    Runtime/Character/System/CharacterCombatSystem.h
    Runtime/Character/System/CharacterMovementSystem.cpp
    Runtime/Character/System/CharacterMovementSystem.h
    Runtime/Character/System/CharacterSystem.cpp
    Runtime/Character/System/CharacterSystem.h
    Runtime/Creature/CreatureRuntime.cpp
    Runtime/Creature/CreatureRuntime.h
    Runtime/World/Command/AttackCharacterCommand.cpp
    Runtime/World/Command/AttackCharacterCommand.h
    Runtime/World/Command/MoveCharacterCommand.cpp
    Runtime/World/Command/MoveCharacterCommand.h
    Runtime/World/Command/WorldCommand.cpp
    Runtime/World/Command/WorldCommand.h
    Runtime/World/System/WorldSystem.cpp
    Runtime/World/System/WorldSystem.h
    Runtime/World/WorldRuntime.cpp
    Runtime/World/WorldRuntime.h)

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
