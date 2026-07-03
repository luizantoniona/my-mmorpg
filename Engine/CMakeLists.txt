cmake_minimum_required(VERSION 3.16)

project(MMORPGEngine LANGUAGES CXX)

file(
  GLOB_RECURSE
  ENGINE_SOURCES
  CONFIGURE_DEPENDS
  Account/*.cpp
  Account/*.h
  Character/*.cpp
  Character/*.h
  Commons/*.cpp
  Commons/*.h
  World/*.cpp
  World/*.h)

add_library(${PROJECT_NAME} STATIC ${ENGINE_SOURCES})

find_package(jsoncpp CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC JsonCpp::JsonCpp)

add_library(MMORPG::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
