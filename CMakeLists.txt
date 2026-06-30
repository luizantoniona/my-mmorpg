cmake_minimum_required(VERSION 3.16)

project(MMORPG LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SERVER "Build MMORPGServer" ON)
option(BUILD_CLIENT "Build MMORPGClient" OFF)
option(BUILD_EDITOR "Build Editor" OFF)

if(BUILD_SERVER)
  add_subdirectory(Engine)
  add_subdirectory(Server)
endif()

if(BUILD_CLIENT)
  add_subdirectory(Engine)
  add_subdirectory(Client)
endif()

if(BUILD_EDITOR)
  add_subdirectory(Editor)
endif()
