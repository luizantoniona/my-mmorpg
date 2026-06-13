add_subdirectory(Engine)

file(GLOB_RECURSE APP_SOURCES
    RegisterTypes.cpp
)

file(GLOB_RECURSE APP_HEADERS
    RegisterTypes.h
)

qt_add_executable(MMORPGClient
    main.cpp
    ${APP_SOURCES}
    ${APP_HEADERS}
)

qt_add_qml_module(MMORPGClient
    URI MMORPGClient
    VERSION 1.0
    QML_FILES
    Main.qml
    ApplicationWindow.qml
    QML/Account/AccountPage.qml
    QML/Game/GamePage.qml
    QML/Login/LoginPage.qml
    QML/Theme/Colors.qml
    QML/Theme/Spaces.qml
)

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/QML/Theme/Colors.qml
    ${CMAKE_CURRENT_SOURCE_DIR}/QML/Theme/Spaces.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE
)

# file(GLOB_RECURSE SVG_FILES
#     RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
#     Assets/*.svg
# )

# qt_add_resources(${PROJECT_NAME}
#     PREFIX "/Assets"
#     BASE "Assets"
#     FILES ${SVG_FILES}
# )

target_link_libraries(MMORPGClient
    # PRIVATE ControlLib
    PRIVATE EngineLib
    PRIVATE Qt6::QuickControls2
    PRIVATE ${QML_MODULES}
)
