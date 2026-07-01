cmake_minimum_required(VERSION 3.16)

project(MMORPGServer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(MMORPGSERVER_BUILD_TESTS "Build tests" OFF)

find_package(Drogon CONFIG REQUIRED)
if(WIN32)
  find_package(unofficial-sqlite3 CONFIG REQUIRED)
endif()
find_package(EngineLib CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)
if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::sqlite3::sqlite3)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE Engine::EngineLib)

include(GNUInstallDirs)
install(
  TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(MMORPGSERVER_BUILD_TESTS)
  enable_testing()
  add_subdirectory(Test)
endif()
