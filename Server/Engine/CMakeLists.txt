file(GLOB_RECURSE ENGINE_SOURCES
    Helper/*.cpp
)

file(GLOB_RECURSE ENGINE_HEADERS
    Helper/*.h
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
