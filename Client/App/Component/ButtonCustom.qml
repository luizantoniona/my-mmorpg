import QtQuick
import QtQuick.Controls
import MMORPGClient

Item {
    id: root

    signal clicked()

    property string vText: ""
    property bool vEnabled: true

        implicitWidth: 160
        implicitHeight: 40

    Rectangle {
        anchors.fill: parent
        radius: 6
        color: mouseArea.pressed ? Colors.primaryPressed : Colors.primary
        border.width: 1
        border.color: Colors.accent

        Text {
            anchors.centerIn: parent
            text: root.vText
            color: Colors.textOnBackground
            font.bold: true
        }

        MouseArea {
            id: mouseArea

            anchors.fill: parent
            enabled: root.vEnabled
            cursorShape: Qt.PointingHandCursor
            onClicked: root.clicked()
        }
    }
}
