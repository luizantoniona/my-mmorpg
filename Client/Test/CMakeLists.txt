find_package(GTest REQUIRED)

add_executable(MMORPGClientTests
    test_example.cpp
)

target_link_libraries(MMORPGClientTests
    PRIVATE GTest::gtest_main
)

add_test(NAME MMORPGClientTests COMMAND MMORPGClientTests)
