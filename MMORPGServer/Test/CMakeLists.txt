find_package(GTest REQUIRED)

add_executable(
  MMORPGServerTests
)

target_link_libraries(MMORPGServerTests PRIVATE MMORPG::MMORPGEngine GTest::gtest_main)

add_test(NAME MMORPGServerTests COMMAND MMORPGServerTests)
