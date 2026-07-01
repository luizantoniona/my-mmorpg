cmake_minimum_required(VERSION 3.16)

project(EngineLib LANGUAGES CXX)

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
  Account/*.cpp
  Character/*.cpp
  Commons/*.cpp
  World/*.cpp
)

file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS
  Account/*.h
  Character/*.h
  Commons/*.h
  World/*.h
)

add_library(EngineLib STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

add_library(Engine::EngineLib ALIAS EngineLib)

target_compile_features(EngineLib PUBLIC cxx_std_20)

target_include_directories(EngineLib
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)
