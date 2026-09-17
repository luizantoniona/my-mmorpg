find_package(GTest REQUIRED)

add_executable(
  MMORPGEditorTests
  ObjectSelectionControlTest.cpp
  TileSelectionControlTest.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGEditor/Editor/Application/World/Object/ObjectSelectionControl.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGEditor/Editor/Application/World/Tile/TileSelectionControl.cpp
)

target_link_libraries(MMORPGEditorTests PRIVATE MMORPG::MMORPGEngine GTest::gtest_main)

add_test(NAME MMORPGEditorTests COMMAND MMORPGEditorTests)
