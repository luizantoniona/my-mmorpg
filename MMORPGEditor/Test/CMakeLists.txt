find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(
  MMORPGEditorTests
  ObjectSelectionControlTest.cpp
  TileSelectionControlTest.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGEditor/Editor/Application/World/Object/ObjectSelectionControl.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGEditor/Editor/Application/World/Tile/TileSelectionControl.cpp
)

target_link_libraries(MMORPGEditorTests PRIVATE MMORPG::MMORPGEngine GTest::gtest_main)

gtest_discover_tests(MMORPGEditorTests DISCOVERY_MODE PRE_TEST)
