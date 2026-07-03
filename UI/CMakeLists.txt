cmake_minimum_required(VERSION 3.16)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick QuickControls2)

qt_add_library(MMORPGUI STATIC)

set_source_files_properties(
  ${CMAKE_CURRENT_SOURCE_DIR}/Qml/Theme/Colors.qml
  ${CMAKE_CURRENT_SOURCE_DIR}/Qml/Theme/Spaces.qml
  PROPERTIES QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(MMORPGUI
  URI MMORPG.UI
  VERSION 1.0
  QML_FILES
  Qml/Component/ButtonCustom.qml
  Qml/Component/InputFieldCustom.qml
  Qml/Component/PanelCustom.qml
  Qml/Theme/Colors.qml
  Qml/Theme/Spaces.qml
)

target_link_libraries(MMORPGUI
  PUBLIC Qt6::Core
  PUBLIC Qt6::QuickControls2
)
