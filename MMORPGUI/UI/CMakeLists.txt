find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick QuickControls2)

qt_add_library(${PROJECT_NAME} STATIC)

add_library(MMORPG::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON AUTORCC ON)

set_source_files_properties(Theme/Colors.qml Theme/Spaces.qml
                            PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(
  ${PROJECT_NAME}
  NO_PLUGIN
  URI
  MMORPGUIComponents
  VERSION
  1.0
  OUTPUT_DIRECTORY
  ${CMAKE_CURRENT_BINARY_DIR}/MMORPGUIComponents
  QML_FILES
  Component/ButtonCustom.qml
  Component/InputFieldCustom.qml
  Component/PanelCustom.qml
  Component/SpacerHorizontal.qml
  Component/SpacerVertical.qml
  Theme/Colors.qml
  Theme/Spaces.qml
  # SOURCES ButtonCustomControl.cpp ButtonCustomControl.h
)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC Qt6::Core
  PUBLIC Qt6::Qml
  PUBLIC Qt6::Quick
  PUBLIC Qt6::QuickControls2)
