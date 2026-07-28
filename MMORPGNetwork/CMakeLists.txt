cmake_minimum_required(VERSION 3.16)

project(MMORPGNetwork LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

set(PROJECT_SOURCES Network/HttpClient.cpp Network/HttpClient.h)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})

find_package(jsoncpp CONFIG REQUIRED)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC Qt6::Core
  PUBLIC Qt6::Network)

add_library(MMORPG::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
