find_package(GTest REQUIRED)

add_executable(MMORPGServerTests test_example.cpp)

target_link_libraries(MMORPGServerTests PRIVATE GTest::gtest_main)

add_test(NAME MMORPGServerTests COMMAND MMORPGServerTests)
