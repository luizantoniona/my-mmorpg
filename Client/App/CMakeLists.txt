add_subdirectory(Engine)

file(GLOB_RECURSE APP_SOURCES RegisterTypes.cpp Application/*.cpp)

file(GLOB_RECURSE APP_HEADERS RegisterTypes.h Application/*.h)

qt_add_executable(MMORPGClientApp main.cpp ${APP_SOURCES} ${APP_HEADERS})

qt_add_qml_module(
  MMORPGClientApp
  URI
  MMORPGClient
  VERSION
  1.0
  QML_FILES
  Main.qml
  ApplicationWindow.qml
  Application/Account/AccountPage.qml
  Application/Game/GamePage.qml
  Application/Login/LoginPage.qml
  Application/Theme/Colors.qml
  Application/Theme/Spaces.qml)

set_source_files_properties(
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/Theme/Colors.qml
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/Theme/Spaces.qml
  PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

# file(GLOB_RECURSE SVG_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} Assets/*.svg
# )

# qt_add_resources(${PROJECT_NAME} PREFIX "/Assets" BASE "Assets" FILES
# ${SVG_FILES} )

target_link_libraries(
  MMORPGClientApp
  PRIVATE EngineLib
  PRIVATE Qt6::QuickControls2
  PRIVATE ${QML_MODULES})
