find_package(GTest REQUIRED)

add_executable(
  MMORPGClientTests
)

target_link_libraries(MMORPGClientTests PRIVATE MMORPG::MMORPGEngine GTest::gtest_main)

add_test(NAME MMORPGClientTests COMMAND MMORPGClientTests)
