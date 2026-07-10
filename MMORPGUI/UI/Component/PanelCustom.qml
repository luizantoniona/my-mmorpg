import QtQuick
import MMORPGUIComponents

Item {
    id: root

    default property alias vContent: content.data

    property color vBackgroundColor: Colors.background2
    property color vBorderColor: Colors.background1

    property int vRadius: 8
    property int vBorderWidth: 8
    property int vMargin: 16

    Rectangle {
        id: panel

        anchors.fill: parent
        anchors.margins: root.vMargin
        radius: root.vRadius
        color: root.vBackgroundColor
        border.width: root.vBorderWidth
        border.color: root.vBorderColor

        Item {
            id: content

            anchors.fill: parent
        }
    }
}
