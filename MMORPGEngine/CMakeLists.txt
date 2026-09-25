cmake_minimum_required(VERSION 3.16)

project(MMORPGEngine LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

option(MMORPGENGINE_BUILD_TESTS "Build tests" OFF)

set(PROJECT_SOURCES
    Account/AccountCharacterDTO.cpp
    Account/AccountCharacterDTO.h
    Account/AccountCharacterModel.cpp
    Account/AccountCharacterModel.h
    Account/AccountDTO.cpp
    Account/AccountDTO.h
    Account/AccountModel.cpp
    Account/AccountModel.h
    Commons/JsonHelper.cpp
    Commons/JsonHelper.h
    Commons/RegisterEngineTypes.cpp
    Commons/RegisterEngineTypes.h
    Commons/Singleton.h
    Data/Animation/AnimationModel.cpp
    Data/Animation/AnimationModel.h
    Data/Item/HandRequirementEnum.h
    Data/Item/HandRequirementHelper.cpp
    Data/Item/HandRequirementHelper.h
    Data/Item/ItemCatalog.cpp
    Data/Item/ItemCatalog.h
    Data/Item/ItemCategoryEnum.h
    Data/Item/ItemCategoryHelper.cpp
    Data/Item/ItemCategoryHelper.h
    Data/Item/ItemFactory.cpp
    Data/Item/ItemFactory.h
    Data/Item/ItemModel.cpp
    Data/Item/ItemModel.h
    Data/Item/ItemSlotEnum.h
    Data/Item/ItemSlotHelper.cpp
    Data/Item/ItemSlotHelper.h
    Data/Item/ItemTypeCatalog.cpp
    Data/Item/ItemTypeCatalog.h
    Data/Item/ItemTypeFactory.cpp
    Data/Item/ItemTypeFactory.h
    Data/Item/ItemTypeModel.cpp
    Data/Item/ItemTypeModel.h
    Data/Monster/MonsterCatalog.cpp
    Data/Monster/MonsterCatalog.h
    Data/Monster/MonsterFactory.cpp
    Data/Monster/MonsterFactory.h
    Data/Monster/MonsterModel.cpp
    Data/Monster/MonsterModel.h
    Data/Monster/MonsterSpawnAreaModel.cpp
    Data/Monster/MonsterSpawnAreaModel.h
    Data/Monster/MonsterSpawnEntryModel.cpp
    Data/Monster/MonsterSpawnEntryModel.h
    Data/Object/ObjectCatalog.cpp
    Data/Object/ObjectCatalog.h
    Data/Object/ObjectFactory.cpp
    Data/Object/ObjectFactory.h
    Data/Object/ObjectInteractionEnum.h
    Data/Object/ObjectInteractionHelper.cpp
    Data/Object/ObjectInteractionHelper.h
    Data/Object/ObjectInteractionModel.cpp
    Data/Object/ObjectInteractionModel.h
    Data/Object/ObjectModel.cpp
    Data/Object/ObjectModel.h
    Data/Object/ObjectSizeModel.cpp
    Data/Object/ObjectSizeModel.h
    Data/Skill/SkillCatalog.cpp
    Data/Skill/SkillCatalog.h
    Data/Skill/SkillFactory.cpp
    Data/Skill/SkillFactory.h
    Data/Skill/SkillNodeModel.cpp
    Data/Skill/SkillNodeModel.h
    Data/Skill/SkillTreeModel.cpp
    Data/Skill/SkillTreeModel.h
    Data/Tile/TileCatalog.cpp
    Data/Tile/TileCatalog.h
    Data/Tile/TileFactory.cpp
    Data/Tile/TileFactory.h
    Data/Tile/TileModel.cpp
    Data/Tile/TileModel.h
    Data/Manifest/ManifestDataDTO.cpp
    Data/Manifest/ManifestDataDTO.h
    Data/Manifest/ManifestDataModel.cpp
    Data/Manifest/ManifestDataModel.h
    Data/Manifest/ManifestDTO.cpp
    Data/Manifest/ManifestDTO.h
    Data/Manifest/ManifestFactory.cpp
    Data/Manifest/ManifestFactory.h
    Data/Manifest/ManifestModel.cpp
    Data/Manifest/ManifestModel.h
    Data/DataFactory.cpp
    Data/DataFactory.h
    Data/DataManager.cpp
    Data/DataManager.h
    Entity/Character/CharacterDTO.cpp
    Entity/Character/CharacterDTO.h
    Entity/Character/CharacterEquipmentDTO.cpp
    Entity/Character/CharacterEquipmentDTO.h
    Entity/Character/CharacterEquipmentModel.cpp
    Entity/Character/CharacterEquipmentModel.h
    Entity/Character/CharacterInventoryDTO.cpp
    Entity/Character/CharacterInventoryDTO.h
    Entity/Character/CharacterInventoryModel.cpp
    Entity/Character/CharacterInventoryModel.h
    Entity/Character/CharacterModel.cpp
    Entity/Character/CharacterModel.h
    Entity/Character/CharacterMoveDTO.cpp
    Entity/Character/CharacterMoveDTO.h
    Entity/Character/CharacterProficiencyModel.cpp
    Entity/Character/CharacterProficiencyModel.h
    Entity/Character/EquipmentSlotEnum.h
    Entity/Character/EquipmentSlotHelper.cpp
    Entity/Character/EquipmentSlotHelper.h
    Entity/Character/OwnCharacterDTO.cpp
    Entity/Character/OwnCharacterDTO.h
    Entity/Character/OwnEquipmentDTO.cpp
    Entity/Character/OwnEquipmentDTO.h
    Entity/Character/OwnInventoryDTO.cpp
    Entity/Character/OwnInventoryDTO.h
    Entity/Creature/CreatureModel.cpp
    Entity/Creature/CreatureModel.h
    Entity/Creature/CreatureDTO.cpp
    Entity/Creature/CreatureDTO.h
    Entity/EntityLeftDTO.cpp
    Entity/EntityLeftDTO.h
    Entity/EntityModel.cpp
    Entity/EntityModel.h
    Entity/EntityPositionModel.cpp
    Entity/EntityPositionModel.h
    Entity/EntitySizeModel.cpp
    Entity/EntitySizeModel.h
    Entity/EntityVitalsModel.cpp
    Entity/EntityVitalsModel.h
    Network/HttpClient.cpp
    Network/HttpClient.h
    Network/WebSocket/ClientMessageTypeEnum.h
    Network/WebSocket/ClientMessageTypeHelper.cpp
    Network/WebSocket/ClientMessageTypeHelper.h
    Network/WebSocket/ServerMessageTypeEnum.h
    Network/WebSocket/ServerMessageTypeHelper.cpp
    Network/WebSocket/ServerMessageTypeHelper.h
    Network/WebSocketClient.cpp
    Network/WebSocketClient.h
    Renderer/Camera/Camera.cpp
    Renderer/Camera/Camera.h
    Renderer/Scene/RenderScene.cpp
    Renderer/Scene/RenderScene.h
    Renderer/Scene/RenderSceneItem.cpp
    Renderer/Scene/RenderSceneItem.h
    Renderer/Scene/TextureCache.cpp
    Renderer/Scene/TextureCache.h
    Renderer/World/RenderWorld.cpp
    Renderer/World/RenderWorld.h
    Renderer/Renderer.cpp
    Renderer/Renderer.h
    Renderer/EntityRenderer.cpp
    Renderer/EntityRenderer.h
    Renderer/ObjectRenderer.cpp
    Renderer/ObjectRenderer.h
    Renderer/TileRenderer.cpp
    Renderer/TileRenderer.h
    UI/Viewport/Viewport.cpp
    UI/Viewport/Viewport.h
    World/Chunk/ChunkModel.cpp
    World/Chunk/ChunkModel.h
    World/Object/WorldObjectModel.cpp
    World/Object/WorldObjectModel.h
    World/Tile/WorldTileModel.cpp
    World/Tile/WorldTileModel.h
    World/FloorFactory.cpp
    World/FloorFactory.h
    World/WorldBasicDTO.cpp
    World/WorldBasicDTO.h
    World/WorldConstants.h
    World/WorldFactory.cpp
    World/WorldFactory.h
    World/WorldModel.cpp
    World/WorldModel.h)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})

find_package(jsoncpp CONFIG REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Core Network Qml Quick WebSockets)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC JsonCpp::JsonCpp
  PUBLIC Qt6::Core
  PUBLIC Qt6::Network
  PUBLIC Qt6::Qml
  PUBLIC Qt6::Quick
  PUBLIC Qt6::WebSockets)

add_library(MMORPG::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)

if(MMORPGENGINE_BUILD_TESTS)
  enable_testing()
  add_subdirectory(Test)
endif()
