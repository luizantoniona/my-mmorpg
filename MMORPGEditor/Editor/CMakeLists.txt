set(PROJECT_SOURCES
    RegisterEditorTypes.cpp
    RegisterEditorTypes.h
    Application/World/Object/ObjectIconProvider.cpp
    Application/World/Object/ObjectIconProvider.h
    Application/World/Object/ObjectPaletteModel.cpp
    Application/World/Object/ObjectPaletteModel.h
    Application/World/Object/ObjectSelectionControl.cpp
    Application/World/Object/ObjectSelectionControl.h
    Application/World/Tile/TileIconProvider.cpp
    Application/World/Tile/TileIconProvider.h
    Application/World/Tile/TilePaletteModel.cpp
    Application/World/Tile/TilePaletteModel.h
    Application/World/Tile/TileSelectionControl.cpp
    Application/World/Tile/TileSelectionControl.h
    Application/World/WorldControl.cpp
    Application/World/WorldControl.h
    Renderer/EditorRenderWorld.cpp
    Renderer/EditorRenderWorld.h)

set_source_files_properties(
  Application/World/BrushMode.qml
  Application/World/Tool/ToolMode.qml
  PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_executable(${PROJECT_NAME} main.cpp ${PROJECT_SOURCES})

qt_add_qml_module(
  ${PROJECT_NAME}
  URI
  MMORPGEditorComponents
  VERSION
  1.0
  QML_FILES
  Main.qml
  Application/EditorWindow.qml
  Application/NavigationBar.qml
  Application/Configuration/ConfigurationPage.qml
  Application/Objects/ObjectsPage.qml
  Application/Tiles/TilesPage.qml
  Application/World/Tool/FloorSelector.qml
  Application/World/Tool/ToolMode.qml
  Application/World/Tool/ToolSelector.qml
  Application/World/Object/ObjectPalette.qml
  Application/World/Tile/TilePalette.qml
  Application/World/BrushMode.qml
  Application/World/EditorSidebar.qml
  Application/World/EditorTopBar.qml
  Application/World/WorldPage.qml)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Qt6::QuickControls2
  PRIVATE MMORPG::MMORPGEngine
  PRIVATE MMORPG::MMORPGUI)
