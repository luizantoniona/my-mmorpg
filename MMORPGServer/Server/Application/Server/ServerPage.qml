import QtQuick
import MMORPGServerComponents
import MMORPGEngine

Item {
    id: root

    Viewport {
        anchors.fill: parent

        cameraPosition: Qt.point(100, 0)

        Behavior on cameraPosition {
            NumberAnimation {
                duration: 1000
            }
        }
    }
}
