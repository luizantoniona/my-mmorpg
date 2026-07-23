import QtQuick
import MMORPGUIComponents

Rectangle {
    id: root

    property color vBackgroundColor: Colors.background1
    property color vBorderColor: Colors.background2
    property int vBorderWidth: Borders.borders2
    property real vRadiusValue: Spaces.spacing8

    color: root.vBackgroundColor
    border {
        color: root.vBorderColor
        width: root.vBorderWidth
    }
    radius: root.vRadiusValue
    clip: true
}
