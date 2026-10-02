import QtQuick
import QtQuick.Layouts
import MMORPGEngine
import MMORPGUIComponents
import MMORPGServerComponents

Item {
    id: root

    property int currentFloor: 0
    readonly property color attackFlashColor: Qt.rgba(Colors.error.r, Colors.error.g, Colors.error.b, 0.55)
    readonly property int attackFlashDurationMs: 220
    readonly property color attackWarningColor: Qt.rgba(Colors.warning.r, Colors.warning.g, Colors.warning.b, 0.45)

    ServerPageControl {
        id: control

        onCharacterStateReceived: function (idCharacter, x, y, z, movementSeconds) {
            serverWorld.addCharacter(idCharacter, x, y, z, movementSeconds)
        }
        onCreatureStateReceived: function (idCreature, x, y, z, movementSeconds) {
            serverWorld.addCreature(idCreature, x, y, z, movementSeconds)
        }
        onCharacterEventAttackStartReceived: function (idCharacter, x, y, z, castSeconds) {
            if (z === root.currentFloor) {
                viewport.addTileWarning(x, y, root.attackWarningColor, castSeconds * 1000)
            }
        }
        onCreatureEventAttackStartReceived: function (idCreature, x, y, z, castSeconds) {
            if (z === root.currentFloor) {
                viewport.addTileWarning(x, y, root.attackWarningColor, castSeconds * 1000)
            }
        }
        onCreatureEventAttackReceived: function (idCreature, x, y, z) {
            if (z === root.currentFloor) {
                viewport.addTileFlash(x, y, root.attackFlashColor, root.attackFlashDurationMs)
            }
        }
        onCharacterEventAttackReceived: function (idCharacter, x, y, z) {
            if (z === root.currentFloor) {
                viewport.addTileFlash(x, y, root.attackFlashColor, root.attackFlashDurationMs)
            }
        }
        onCharacterEventLeaveReceived: function (idCharacter) {
            serverWorld.removeCharacter(idCharacter)
        }
        onCreatureEventLeaveReceived: function (idCreature) {
            serverWorld.removeCreature(idCreature)
        }
    }

    ServerRenderWorld {
        id: serverWorld
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        ServerSidebar {
            id: sidebar

            Layout.fillHeight: true
            width: 280

            control: control
            worldName: control.worldName
            worldWidth: control.worldWidth
            worldHeight: control.worldHeight
            currentFloor: root.currentFloor

            onCharacterCenterRequested: function (x, y, z) {
                root.currentFloor = z
                viewport.centerCameraOnTile(x, y)
            }
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 0

            Rectangle {
                Layout.fillWidth: true
                height: 40
                color: Colors.background2

                FloorSelector {
                    anchors.fill: parent
                    floors: control.floors
                    currentFloor: root.currentFloor

                    onFloorRequested: function (z) {
                        root.currentFloor = z
                    }
                }
            }

            Rectangle {
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
                    renderWorld: serverWorld
                    activeFloor: root.currentFloor
                }
            }
        }
    }

    Component.onCompleted: {
        viewport.centerCameraOnTile(control.worldWidth / 2, control.worldHeight / 2)
    }

    Shortcut {
        sequence: "W"
        onActivated: viewport.moveCameraByTiles(0, -1)
    }

    Shortcut {
        sequence: "S"
        onActivated: viewport.moveCameraByTiles(0, 1)
    }

    Shortcut {
        sequence: "A"
        onActivated: viewport.moveCameraByTiles(-1, 0)
    }

    Shortcut {
        sequence: "D"
        onActivated: viewport.moveCameraByTiles(1, 0)
    }
}
