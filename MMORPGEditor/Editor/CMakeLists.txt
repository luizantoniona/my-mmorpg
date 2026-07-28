set(PROJECT_SOURCES
    RegisterTypes.cpp RegisterTypes.h Application/EditorPageControl.cpp
    Application/EditorPageControl.h)

qt_add_executable(${PROJECT_NAME} main.cpp ${PROJECT_SOURCES})

qt_add_qml_module(
  ${PROJECT_NAME}
  URI
  MMORPGEditorComponents
  VERSION
  1.0
  QML_FILES
  Main.qml
  Application/EditorPage.qml)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Qt6::QuickControls2
  PRIVATE MMORPG::MMORPGEngine
  PRIVATE MMORPG::MMORPGUI)
