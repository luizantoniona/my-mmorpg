import QtQuick
import MMORPGUIComponents

Text {
    id: root

    property string vText: ""
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
    text: vText
}
