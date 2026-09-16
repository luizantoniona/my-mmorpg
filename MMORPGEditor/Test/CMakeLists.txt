find_package(GTest REQUIRED)

add_executable(
  MMORPGEditorTests
)

target_link_libraries(MMORPGEditorTests PRIVATE MMORPG::MMORPGEngine GTest::gtest_main)

add_test(NAME MMORPGEditorTests COMMAND MMORPGEditorTests)
