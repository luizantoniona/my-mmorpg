set(PROJECT_SOURCES
    RegisterEditorTypes.cpp
    RegisterEditorTypes.h
    Application/World/WorldControl.cpp
    Application/World/WorldControl.h
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
    Renderer/EditorRenderWorld.cpp
    Renderer/EditorRenderWorld.h)

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
  Application/Navigation/NavigationBar.qml
  Application/World/WorldPage.qml
  Application/Tiles/TilesPage.qml
  Application/Objects/ObjectsPage.qml)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Qt6::QuickControls2
  PRIVATE MMORPG::MMORPGEngine
  PRIVATE MMORPG::MMORPGUI)
