file(GLOB_RECURSE ENGINE_SOURCES
    Character/*.cpp
    Commons/*.cpp
    Database/*.cpp
    Network/*.cpp
    User/*.cpp
    World/*.cpp
)

file(GLOB_RECURSE ENGINE_HEADERS
    Character/*.cpp
    Commons/*.h
    Database/*.h
    Network/*.h
    User/*.h
    World/*.h
)

add_library(EngineLib STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

target_include_directories(EngineLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(EngineLib PUBLIC Drogon::Drogon)

if(WIN32)
    target_link_libraries(EngineLib PUBLIC unofficial::sqlite3::sqlite3)
endif(WIN32)
