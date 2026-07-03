set(APP_SOURCES RegisterTypes.cpp RegisterTypes.h Editor/EditorPageControl.cpp
                Editor/EditorPageControl.h)

qt_add_executable(MMORPGEditorApp main.cpp ${APP_SOURCES})

qt_add_qml_module(
  MMORPGEditorApp
  URI
  MMORPGEditor
  VERSION
  1.0
  IMPORTS
  MMORPG.UI
  QML_FILES
  Main.qml
  Editor/EditorPage.qml)

target_link_libraries(
  MMORPGEditorApp
  PRIVATE Qt6::QuickControls2
  PRIVATE MMORPGUI)
