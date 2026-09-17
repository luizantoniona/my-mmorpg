find_package(GTest REQUIRED)

add_executable(
  MMORPGServerTests
  ChunkCoordinateTest.cpp
  EventBusTest.cpp
  WorldRuntimeTest.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Event/EventBus.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Event/WorldEvent.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Manager/ChunkCoordinate.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/CharacterRuntime.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/CharacterSystem.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/CharacterVitalsSystem.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/CreatureRuntime.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/WorldRuntime.cpp
)

target_link_libraries(MMORPGServerTests PRIVATE MMORPG::MMORPGEngine GTest::gtest_main)

add_test(NAME MMORPGServerTests COMMAND MMORPGServerTests)
