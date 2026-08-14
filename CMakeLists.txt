cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
  file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" VCPKG_ROOT_CMAKE)
  set(CMAKE_TOOLCHAIN_FILE
      "${VCPKG_ROOT_CMAKE}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

project(MMORPG LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SERVER "Build MMORPGServer" ON)
option(BUILD_CLIENT "Build MMORPGClient" ON)
option(BUILD_EDITOR "Build MMORPGEditor" ON)

add_subdirectory(MMORPGEngine)

add_subdirectory(MMORPGUI)
list(APPEND QML_IMPORT_PATH "${CMAKE_BINARY_DIR}/MMORPGUI/UI")

if(BUILD_SERVER)
  add_subdirectory(MMORPGServer)
endif()

if(BUILD_CLIENT)
  add_subdirectory(MMORPGClient)
  list(APPEND QML_IMPORT_PATH "${CMAKE_BINARY_DIR}/MMORPGClient/Client")
endif()

if(BUILD_EDITOR)
  add_subdirectory(MMORPGEditor)
  list(APPEND QML_IMPORT_PATH "${CMAKE_BINARY_DIR}/MMORPGEditor/Editor")
endif()

set(QML_IMPORT_PATH
    "${QML_IMPORT_PATH}"
    CACHE STRING "QML import paths" FORCE)
