find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick QuickEffects QuickControls2)

qt_add_library(${PROJECT_NAME} STATIC)

add_library(MMORPG::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON AUTORCC ON)

file(GLOB_RECURSE SVG_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} Assets/*.svg)

qt_add_resources(${PROJECT_NAME} "icons" PREFIX "/Assets" BASE "Assets" FILES ${SVG_FILES})

set_source_files_properties(
  Theme/Borders.qml
  Theme/Colors.qml
  Theme/Fonts.qml
  Theme/Icons.qml
  Theme/Radiuses.qml
  Theme/Spaces.qml
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
  Component/Button/ButtonBase.qml
  Component/Icon/IconBase.qml
  Component/Input/InputBase.qml
  Component/Loading/LoadingBase.qml
  Component/Loading/LoadingProgress.qml
  Component/Panel/PanelBase.qml
  Component/Panel/PanelContent.qml
  Component/Panel/PanelFrame.qml
  Component/Panel/PanelHeader.qml
  Component/Spacer/SpacerHorizontal.qml
  Component/Spacer/SpacerVertical.qml
  Component/Text/TextSubTitle.qml
  Component/Text/TextTitle.qml
  Theme/Borders.qml
  Theme/Colors.qml
  Theme/Fonts.qml
  Theme/Icons.qml
  Theme/Radiuses.qml
  Theme/Spaces.qml
  # SOURCES ButtonCustomControl.cpp ButtonCustomControl.h
)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC Qt6::Core
  PUBLIC Qt6::Qml
  PUBLIC Qt6::Quick
  PUBLIC Qt6::QuickEffects
  PUBLIC Qt6::QuickControls2)
