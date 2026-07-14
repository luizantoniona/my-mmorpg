import QtQuick
import MMORPGUIComponents

Item {
    id: root

    default property alias vContent: content.data

    readonly property alias panel: panel

    property color vBackgroundColor: Colors.background1
    property color vBorderColor: Colors.background2

    property int vBorder: Borders.borders2
    property int vMargins: Spaces.spacing8
    property int vPadding: Spaces.spacing8
    property int vRadius: Spaces.spacing8

    implicitWidth: 100
    implicitHeight: 100

    Rectangle {
        id: panel

        anchors{
            fill: parent
            margins: root.vMargins
        }
        border {
            width: root.vBorder
            color: root.vBorderColor
        }
        clip: true
        color: root.vBackgroundColor
        radius: root.vRadius

        Item {
            id: content

            anchors.fill: parent
            anchors.margins: root.vPadding
        }
    }
}
