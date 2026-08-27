import QtQuick
import MMORPGServerComponents
import MMORPGEngine

Item {
    id: root

    focus: true

    ServerRenderWorld {
        id: serverWorld
    }

    Viewport {
        id: viewport

        anchors.fill: parent
        renderWorld: serverWorld
    }

    Component.onCompleted: {
        forceActiveFocus()
        viewport.centerCameraOnTile(16, 16)
    }

    Keys.onPressed: function (event) {
        console.log("KEY:", event.key)
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
        event.accepted = true
    }
}
