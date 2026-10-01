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
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/Character/System/CharacterCombatSystem.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/Character/System/CharacterMovementSystem.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/Character/System/CharacterSystem.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/Character/System/CharacterVitalsSystem.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/Creature/CreatureRuntime.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/World/Command/WorldCommand.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/World/System/WorldCombatSystem.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/World/System/WorldCreatureSystem.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/World/System/WorldMovementSystem.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/World/System/WorldSpawnSystem.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/World/System/WorldSystem.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/World/WorldRuntime.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGServer/Server/Runtime/World/WorldSpatialIndex.cpp
)

target_link_libraries(MMORPGServerTests PRIVATE MMORPG::MMORPGEngine GTest::gtest_main)

gtest_discover_tests(MMORPGServerTests DISCOVERY_MODE PRE_TEST)
