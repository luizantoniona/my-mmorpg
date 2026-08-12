cmake_minimum_required(VERSION 3.16)

project(MMORPGEngine LANGUAGES CXX)

set(PROJECT_SOURCES
    Commons/JsonHelper.cpp
    Commons/JsonHelper.h
    Commons/Singleton.h
    Core/Account/AccountDTO.cpp
    Core/Account/AccountDTO.h
    Core/Account/AccountModel.cpp
    Core/Account/AccountModel.h
    Core/Character/CharacterDTO.cpp
    Core/Character/CharacterDTO.h
    Core/Character/CharacterModel.cpp
    Core/Character/CharacterModel.h
    Core/World/Catalog/GroundCatalog.cpp
    Core/World/Catalog/GroundCatalog.h
    Core/World/Model/FloorModel.cpp
    Core/World/Model/FloorModel.h
    Core/World/Model/GroundModel.cpp
    Core/World/Model/GroundModel.h
    Core/World/Model/TileModel.cpp
    Core/World/Model/TileModel.h
    Core/World/Model/WorldModel.cpp
    Core/World/Model/WorldModel.h
    Core/World/WorldFactory.cpp
    Core/World/WorldFactory.h
    ManifestDataDTO.cpp
    ManifestDataDTO.h
    ManifestDataModel.cpp
    ManifestDataModel.h
    ManifestDTO.cpp
    ManifestDTO.h
    ManifestModel.cpp
    ManifestModel.h)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})

find_package(jsoncpp CONFIG REQUIRED)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC JsonCpp::JsonCpp)

add_library(MMORPG::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
