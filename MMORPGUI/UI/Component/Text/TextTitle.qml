import QtQuick
import MMORPGUIComponents

Text {
    id: root

    property color vTextColor: Colors.text
    property int vFontSize: 14

    color: root.vTextColor
    font {
        family: Fonts.familyTitle
        pixelSize: root.vFontSize
        bold: true
    }
    verticalAlignment: Text.AlignVCenter
    elide: Text.ElideRight
}
