import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGEditorComponents
import MMORPGEditorControls

Item {
    id: root

    WorldPageControl {
        id: control
    }

    Component.onCompleted: function () {
        control.loadWorld()
    }

    Keys.onPressed: function (event) {
        switch (event.key) {
        case Qt.Key_W:
            // TODO: Move up (Chunk or tile?)
            break
        case Qt.Key_A:
            // TODO: Move left (Chunk or tile?)
            break
        case Qt.Key_S:
            // TODO: Move down (Chunk or tile?)
            break
        case Qt.Key_D:
            // TODO: Move right (Chunk or tile?)
            break
        case Qt.Key_E:
            // TODO: Move floor up
            break
        case Qt.Key_Q:
            // TODO: Move floor down
            break
        default:
            break
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: Spaces.spacing8

        WorldPageHeader {
            Layout.fillWidth: true
            height: 80

            Rectangle {
                anchors.fill: parent
                color: "red"
            }
        }

        // TODO: Change for map view component
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                anchors.fill: parent
                color: "blue"
            }
        }
    }
}
