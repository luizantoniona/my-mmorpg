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
        radius: 8
        color: mouseArea.pressed ? Colors.primary2 : Colors.primary1
        border.width: 2
        border.color: Colors.primary0

        Text {
            anchors.centerIn: parent
            text: root.vText
            color: Colors.text
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
