import QtQuick
import QtQuick.Layouts
import MMORPGEngine
import MMORPGUIComponents
import MMORPGServerComponents

Item {
    id: root

    property int currentFloor: 0

    ServerPageControl {
        id: control
    }

    ServerRenderWorld {
        id: serverWorld
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0
            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "transparent"
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
