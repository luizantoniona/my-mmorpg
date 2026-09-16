find_package(GTest REQUIRED)

add_executable(
  MMORPGEngineTests
)

target_link_libraries(MMORPGEngineTests PRIVATE MMORPG::MMORPGEngine GTest::gtest_main)

add_test(NAME MMORPGEngineTests COMMAND MMORPGEngineTests)
