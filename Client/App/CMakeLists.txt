set(APP_SOURCES
    RegisterTypes.cpp
    RegisterTypes.h
    Application/Account/AccountPageControl.cpp
    Application/Account/AccountPageControl.h
    Application/Game/GamePageControl.cpp
    Application/Game/GamePageControl.h
    Application/Login/LoginPageControl.cpp
    Application/Login/LoginPageControl.h
    Manager/AccountManager.cpp
    Manager/AccountManager.h
    Manager/ServerManager.cpp
    Manager/ServerManager.h)

qt_add_executable(MMORPGClientApp main.cpp ${APP_SOURCES})

qt_add_qml_module(
  MMORPGClientApp
  URI
  MMORPGClient
  VERSION
  1.0
  QML_FILES
  Main.qml
  Application/ApplicationWindow.qml
  Application/Account/AccountPage.qml
  Application/Game/GamePage.qml
  Application/Login/LoginPage.qml)

# file(GLOB_RECURSE SVG_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} Assets/*.svg
# )

# qt_add_resources(${PROJECT_NAME} PREFIX "/Assets" BASE "Assets" FILES
# ${SVG_FILES} )

target_link_libraries(
  MMORPGClientApp
  PRIVATE Qt6::QuickControls2
  PRIVATE MMORPG::MMORPGEngine
  PRIVATE MMORPGUI
  PRIVATE ${QML_MODULES})
