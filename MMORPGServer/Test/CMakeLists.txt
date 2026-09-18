find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(
  MMORPGServerTests
  ChunkCoordinateTest.cpp
  EventBusTest.cpp
  WorldRuntimeTest.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Event/EventBus.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Event/WorldEvent.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Manager/ChunkCoordinate.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/Character/CharacterRuntime.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/Character/System/CharacterSystem.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/Creature/CreatureRuntime.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/World/WorldRuntime.cpp
)

target_link_libraries(MMORPGServerTests PRIVATE MMORPG::MMORPGEngine GTest::gtest_main)

gtest_discover_tests(MMORPGServerTests DISCOVERY_MODE PRE_TEST)
