set(PROJECT_SOURCES
    RegisterClientTypes.cpp
    RegisterClientTypes.h
    Application/Account/AccountPageControl.cpp
    Application/Account/AccountPageControl.h
    Application/Game/Component/CharacterEquipmentPanelControl.cpp
    Application/Game/Component/CharacterEquipmentPanelControl.h
    Application/Game/Component/CharacterInventoryPanelControl.cpp
    Application/Game/Component/CharacterInventoryPanelControl.h
    Application/Game/GamePageControl.cpp
    Application/Game/GamePageControl.h
    Application/Login/LoginPageControl.cpp
    Application/Login/LoginPageControl.h
    Application/Sync/SyncPageControl.cpp
    Application/Sync/SyncPageControl.h
    Manager/AccountManager.cpp
    Manager/AccountManager.h
    Manager/ServerManager.cpp
    Manager/ServerManager.h
    Renderer/ClientRenderWorld.cpp
    Renderer/ClientRenderWorld.h)

qt_add_executable(${PROJECT_NAME} main.cpp ${PROJECT_SOURCES})

qt_add_qml_module(
  ${PROJECT_NAME}
  URI
  MMORPGClientComponents
  VERSION
  1.0
  QML_FILES
  Main.qml
  Application/ClientWindow.qml
  Application/Account/AccountPage.qml
  Application/Game/GamePage.qml
  Application/Game/Component/CharacterVitalsPanel.qml
  Application/Game/Component/CharacterEquipmentPanel.qml
  Application/Game/Component/GamePanelBase.qml
  Application/Game/Component/GameActionBar.qml
  Application/Game/Component/CharacterInventoryPanel.qml
  Application/Game/Component/ItemSlot.qml
  Application/Game/Component/VitalBar.qml
  Application/Login/LoginPage.qml
  Application/Login/Component/AccountPanel.qml
  Application/Login/Component/ServerPanel.qml
  Application/Login/Component/ServerStatus.qml
  Application/Sync/SyncPage.qml)

# file(GLOB_RECURSE SVG_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} Assets/*.svg
# )

# qt_add_resources(${PROJECT_NAME} PREFIX "/Assets" BASE "Assets" FILES
# ${SVG_FILES} )

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Qt6::QuickControls2
  PRIVATE MMORPG::MMORPGEngine
  PRIVATE MMORPG::MMORPGUI)
