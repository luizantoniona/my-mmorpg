cmake_minimum_required(VERSION 3.16)

project(MMORPGEngine LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

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
    Data/Object/ObjectCatalog.cpp
    Data/Object/ObjectCatalog.h
    Data/Object/ObjectModel.cpp
    Data/Object/ObjectModel.h
    Data/Object/ObjectSizeModel.cpp
    Data/Object/ObjectSizeModel.h
    Data/Tile/TileCatalog.cpp
    Data/Tile/TileCatalog.h
    Data/Tile/TileModel.cpp
    Data/Tile/TileModel.h
    Data/Manifest/ManifestDataDTO.cpp
    Data/Manifest/ManifestDataDTO.h
    Data/Manifest/ManifestDataModel.cpp
    Data/Manifest/ManifestDataModel.h
    Data/Manifest/ManifestDTO.cpp
    Data/Manifest/ManifestDTO.h
    Data/Manifest/ManifestModel.cpp
    Data/Manifest/ManifestModel.h
    Data/DataFactory.cpp
    Data/DataFactory.h
    Data/DataManager.cpp
    Data/DataManager.h
    Entity/Character/CharacterModel.cpp
    Entity/Character/CharacterModel.h
    Entity/Creature/CreatureModel.cpp
    Entity/Creature/CreatureModel.h
    Entity/EntityModel.cpp
    Entity/EntityModel.h
    Entity/EntityOrientationModel.cpp
    Entity/EntityOrientationModel.h
    Entity/EntityPositionModel.cpp
    Entity/EntityPositionModel.h
    Entity/EntitySizeModel.cpp
    Entity/EntitySizeModel.h
    Network/HttpClient.cpp
    Network/HttpClient.h
    Renderer/Camera/Camera.cpp
    Renderer/Camera/Camera.h
    Renderer/Scene/RenderScene.cpp
    Renderer/Scene/RenderScene.h
    Renderer/Scene/RenderSceneItem.cpp
    Renderer/Scene/RenderSceneItem.h
    Renderer/World/RenderWorld.cpp
    Renderer/World/RenderWorld.h
    Renderer/Renderer.cpp
    Renderer/Renderer.h
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
    World/WorldFactory.cpp
    World/WorldFactory.h
    World/WorldModel.cpp
    World/WorldModel.h)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})

find_package(jsoncpp CONFIG REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Core Network Qml Quick)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC JsonCpp::JsonCpp
  PUBLIC Qt6::Core
  PUBLIC Qt6::Network
  PUBLIC Qt6::Qml
  PUBLIC Qt6::Quick)

add_library(MMORPG::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
