cmake_minimum_required(VERSION 3.16)

project(MMORPGEngine LANGUAGES CXX)

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
    Commons/Singleton.h
    Entity/EntityModel.cpp
    Entity/EntityModel.h
    Entity/EntityPositionModel.cpp
    Entity/EntityPositionModel.h
    Entity/Character/CharacterModel.cpp
    Entity/Character/CharacterModel.h
    Entity/Creature/CreatureModel.cpp
    Entity/Creature/CreatureModel.h
    Factory/WorldFactory.cpp
    Factory/WorldFactory.h
    Manifest/ManifestDataDTO.cpp
    Manifest/ManifestDataDTO.h
    Manifest/ManifestDataModel.cpp
    Manifest/ManifestDataModel.h
    Manifest/ManifestDTO.cpp
    Manifest/ManifestDTO.h
    Manifest/ManifestModel.cpp
    Manifest/ManifestModel.h
    World/FloorModel.cpp
    World/FloorModel.h
    World/GroundModel.cpp
    World/GroundModel.h
    World/TileModel.cpp
    World/TileModel.h
    World/WorldModel.cpp
    World/WorldModel.h)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})

find_package(jsoncpp CONFIG REQUIRED)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC JsonCpp::JsonCpp)

add_library(MMORPG::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
