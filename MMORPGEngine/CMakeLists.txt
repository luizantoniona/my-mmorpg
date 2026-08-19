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
    Data/Catalog/GroundCatalog.cpp
    Data/Catalog/GroundCatalog.h
    Data/Manifest/ManifestDataDTO.cpp
    Data/Manifest/ManifestDataDTO.h
    Data/Manifest/ManifestDataModel.cpp
    Data/Manifest/ManifestDataModel.h
    Data/Manifest/ManifestDTO.cpp
    Data/Manifest/ManifestDTO.h
    Data/Manifest/ManifestModel.cpp
    Data/Manifest/ManifestModel.h
    Entity/EntityModel.cpp
    Entity/EntityModel.h
    Entity/EntityPositionModel.cpp
    Entity/EntityPositionModel.h
    Entity/Character/CharacterModel.cpp
    Entity/Character/CharacterModel.h
    Entity/Creature/CreatureModel.cpp
    Entity/Creature/CreatureModel.h
    Network/HttpClient.cpp
    Network/HttpClient.h
    Renderer/Camera/Camera.cpp
    Renderer/Camera/Camera.h
    Renderer/Scene/RenderScene.cpp
    Renderer/Scene/RenderScene.h
    Renderer/World/RenderWorld.cpp
    Renderer/World/RenderWorld.h
    Renderer/Renderer.cpp
    Renderer/Renderer.h
    Renderer/TileRenderer.cpp
    Renderer/TileRenderer.h
    UI/Viewport/Viewport.cpp
    UI/Viewport/Viewport.h
    World/Chunk/ChunkModel.cpp
    World/Chunk/ChunkModel.h
    World/Tile/GroundModel.cpp
    World/Tile/GroundModel.h
    World/Tile/TileModel.cpp
    World/Tile/TileModel.h
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
