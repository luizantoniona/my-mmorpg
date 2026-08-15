import QtQuick
import MMORPGServerComponents
import MMORPGEngine

Item {
    id: root

    ServerRenderWorld {
        id: serverWorld
    }

    Viewport {
        anchors.fill: parent
        renderWorld: serverWorld

        cameraPosition: Qt.point(0, 0)
    }
}
