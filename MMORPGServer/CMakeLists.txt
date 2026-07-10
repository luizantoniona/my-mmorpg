cmake_minimum_required(VERSION 3.16)

project(MMORPGServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(MMORPGSERVER_BUILD_TESTS "Build tests" OFF)

find_package(Drogon CONFIG REQUIRED)
if(WIN32)
  find_package(unofficial-sqlite3 CONFIG REQUIRED)
endif()

add_subdirectory(Server)

if(MMORPGSERVER_BUILD_TESTS)
  enable_testing()
  add_subdirectory(Test)
endif()
