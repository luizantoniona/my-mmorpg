find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick QuickControls2)

qt_add_library(MMORPGUI STATIC)

add_library(MMORPG::MMORPGUI ALIAS MMORPGUI)

set_target_properties(MMORPGUI PROPERTIES AUTOMOC ON AUTORCC ON)

set_source_files_properties(Qml/Theme/Colors.qml Qml/Theme/Spaces.qml
                            PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(
  MMORPGUI
  URI
  MMORPG.UI
  VERSION
  1.0
  QML_FILES
  Qml/Component/ButtonCustom.qml
  Qml/Component/InputFieldCustom.qml
  Qml/Component/PanelCustom.qml
  Qml/Theme/Colors.qml
  Qml/Theme/Spaces.qml
  # SOURCES ButtonCustomControl.cpp ButtonCustomControl.h
)

target_link_libraries(MMORPGUI PUBLIC Qt6::Core Qt6::Qml Qt6::Quick
                                      Qt6::QuickControls2)

qt_finalize_target(MMORPGUI)
