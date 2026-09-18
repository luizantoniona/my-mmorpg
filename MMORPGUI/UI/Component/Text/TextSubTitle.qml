import QtQuick
import MMORPGUIComponents

Text {
    id: root

    property string vText: ""
    property color vTextColor: Colors.text

    color: root.vTextColor
    font: Fonts.titleMedium
    verticalAlignment: Text.AlignVCenter
    elide: Text.ElideRight
    text: vText
}
