find_package(GTest REQUIRED)

add_executable(
  MMORPGClientTests
  ClientRenderWorldTest.cpp
  ${CMAKE_SOURCE_DIR}/MMORPGClient/Client/Renderer/ClientRenderWorld.cpp
)

target_link_libraries(MMORPGClientTests PRIVATE MMORPG::MMORPGEngine GTest::gtest_main)

add_test(NAME MMORPGClientTests COMMAND MMORPGClientTests)
