import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MMORPGEngine
import MMORPGUIComponents
import MMORPGEditorComponents
import MMORPGEditorControls

Item {
    id: root

    property int activeTileType: -1
    property int activeObjectType: -1

    property int activeBrush: BrushMode.Tile
    property int toolMode: ToolMode.Select

    property int currentFloor: 0

    readonly property color spawnOverlayColor: Qt.rgba(Colors.warning.r, Colors.warning.g, Colors.warning.b, 0.35)

    function refreshSpawnOverlay() {
        viewport.setOverlayRects(worldControl.spawnAreas(root.currentFloor), root.spawnOverlayColor)
    }

    TileSelectionControl {
        id: selectionControl

        onSelectionChanged: function () {
            if (!selectionControl.hasSelection) {
                viewport.clearHighlight()
                return
            }
            viewport.setHighlightedTile(selectionControl.x, selectionControl.y)
        }
    }

    ObjectSelectionControl {
        id: objectSelectionControl

        onSelectionChanged: function () {
            if (!objectSelectionControl.hasSelection) {
                viewport.clearHighlight()
                return
            }
            viewport.setHighlightedTile(objectSelectionControl.x, objectSelectionControl.y)
        }
    }

    WorldPageControl {
        id: worldControl
    }

    EditorRenderWorld {
        id: editorWorld
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        EditorSidebar {
            id: editorBar

            Layout.fillHeight: true
            width: 220

            onTileSelected: function (type) {
                root.activeTileType = type
                root.activeBrush = BrushMode.Tile
            }

            onObjectSelected: function (type) {
                root.activeObjectType = type
                root.activeBrush = BrushMode.Object
            }
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 0

            EditorTopBar {
                id: toolBar

                Layout.fillWidth: true
                height: 40
                mode: root.toolMode
                floors: worldControl.floors
                currentFloor: root.currentFloor

                onModeRequested: function (mode) {
                    root.toolMode = mode

                    if (mode === ToolMode.Paint || mode === ToolMode.Spawn) {
                        selectionControl.clearSelection()
                        objectSelectionControl.clearSelection()
                    }
                }

                onFloorRequested: function (z) {
                    root.currentFloor = z
                    selectionControl.clearSelection()
                    objectSelectionControl.clearSelection()
                    viewport.clearHighlight()
                    root.refreshSpawnOverlay()
                }

                onAddFloorRequested: function (above) {
                    const ok = worldControl.addFloor(above)

                    if (ok) {
                        editorWorld.world = worldControl.world
                        const floors = worldControl.floors
                        root.currentFloor = above ? Math.max(...floors) : Math.min(...floors)
                        selectionControl.clearSelection()
                        objectSelectionControl.clearSelection()
                        viewport.clearHighlight()
                        root.refreshSpawnOverlay()
                    }
                }

                onRemoveFloorRequested: function (z) {
                    removeFloorDialog.pendingZ = z
                    removeFloorDialog.open()
                }

                onSaveRequested: function () {
                    if (worldControl.saveWorld()) {
                        console.log("World saved")
                    } else {
                        console.log("Failed to save world")
                    }
                }
            }

            Rectangle {
                id: viewportFrame

                Layout.fillHeight: true
                Layout.fillWidth: true
                color: Colors.transparent
                border.color: Colors.border
                border.width: Borders.border1
                clip: true

                Viewport {
                    id: viewport

                    anchors.fill: parent
                    anchors.margins: Borders.border1
                    renderWorld: editorWorld
                    activeFloor: root.currentFloor
                    cursorShape: {
                        if (root.toolMode === ToolMode.Paint) {
                            return Qt.CrossCursor
                        }
                        if (root.toolMode === ToolMode.Spawn) {
                            return Qt.PointingHandCursor
                        }
                        return Qt.ArrowCursor
                    }

                    onTileClicked: function (x, y, z) {
                        if (root.toolMode === ToolMode.Paint) {
                            if (root.activeBrush === BrushMode.Tile && root.activeTileType >= 0) {
                                worldControl.paintTile(x, y, z, root.activeTileType)
                            } else if (root.activeBrush === BrushMode.Object && root.activeObjectType >= 0) {
                                worldControl.paintObject(x, y, z, root.activeObjectType)
                            }
                            return
                        }

                        if (root.toolMode === ToolMode.Spawn) {
                            const existing = worldControl.spawnAreaAt(x, y, z)

                            if (Object.keys(existing).length > 0) {
                                worldControl.removeSpawnArea(x, y, z)
                                spawnAreaPanel.loadFrom(existing.width, existing.height, existing.respawnSeconds, existing.creatures)
                            } else {
                                worldControl.paintSpawnArea(x, y, z, spawnAreaPanel.vWidth, spawnAreaPanel.vHeight, spawnAreaPanel.vRespawnSeconds, spawnAreaPanel.creaturesList())
                            }

                            root.refreshSpawnOverlay()
                            return
                        }

                        if (editorWorld.hasObject(x, y, z)) {
                            selectionControl.clearSelection()
                            objectSelectionControl.selectObject(x, y, z)
                        } else {
                            objectSelectionControl.clearSelection()
                            selectionControl.selectTile(x, y, z)
                        }
                    }

                    onTileRightClicked: function (x, y, z) {
                        if (root.toolMode !== ToolMode.Spawn) {
                            return
                        }

                        if (worldControl.removeSpawnArea(x, y, z)) {
                            root.refreshSpawnOverlay()
                        }
                    }
                }

                SpawnAreaPanel {
                    id: spawnAreaPanel

                    anchors {
                        top: parent.top
                        right: parent.right
                        margins: Spaces.spacing8
                    }
                    visible: root.toolMode === ToolMode.Spawn
                }
            }
        }
    }

    Dialog {
        id: removeFloorDialog

        property int pendingZ: 0

        anchors.centerIn: parent
        modal: true
        title: "Remove Floor"
        standardButtons: Dialog.Yes | Dialog.No

        Text {
            color: Colors.text
            font: Fonts.bodyBold
            text: "Remove floor Z" + removeFloorDialog.pendingZ + "? This cannot be undone."
        }

        onAccepted: {
            const ok = worldControl.removeFloor(removeFloorDialog.pendingZ)

            if (ok) {
                editorWorld.world = worldControl.world
                const floors = worldControl.floors
                root.currentFloor = floors.length > 0 ? floors[0] : 0
                selectionControl.clearSelection()
                objectSelectionControl.clearSelection()
                viewport.clearHighlight()
                root.refreshSpawnOverlay()
            }
        }
    }

    Component.onCompleted: function () {
        worldControl.loadWorld()
        editorWorld.world = worldControl.world
        forceActiveFocus()
        viewport.centerCameraOnTile(worldControl.worldWidth / 2, worldControl.worldHeight / 2)
        root.refreshSpawnOverlay()
    }

    Keys.onPressed: function (event) {
        switch (event.key) {
        case Qt.Key_W:
            viewport.moveCameraByTiles(0, -1)
            break
        case Qt.Key_S:
            viewport.moveCameraByTiles(0, 1)
            break
        case Qt.Key_A:
            viewport.moveCameraByTiles(-1, 0)
            break
        case Qt.Key_D:
            viewport.moveCameraByTiles(1, 0)
            break
        }
    }
}
