file(GLOB_RECURSE ENGINE_SOURCES
    Account/*.cpp
    Character/*.cpp
    Commons/*.cpp
    Database/*.cpp
    Network/*.cpp
    World/*.cpp
)

file(GLOB_RECURSE ENGINE_HEADERS
    Account/*.h
    Character/*.h
    Commons/*.h
    Database/*.h
    Network/*.h
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
