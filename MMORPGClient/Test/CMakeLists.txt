find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(
  MMORPGClientTests
  ClientRenderWorldTest.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGClient/Client/Renderer/ClientRenderWorld.cpp
)

target_link_libraries(MMORPGClientTests PRIVATE MMORPG::MMORPGEngine GTest::gtest_main)

gtest_discover_tests(MMORPGClientTests DISCOVERY_MODE PRE_TEST)
