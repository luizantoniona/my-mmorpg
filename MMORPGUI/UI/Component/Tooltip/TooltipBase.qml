import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MMORPGUIComponents

ToolTip {
    id: root

    property string vTitle: ""
    property string vText: ""

    x: (parent ? parent.width - width : 0) / 2
    y: -height - Spaces.spacing8
    delay: 400
    padding: Spaces.spacing8

    background: PanelFrame {
        vBackgroundColor: Colors.background0
        vBorderColor: Colors.border
        vBorderWidth: Borders.border1
        vRadiusValue: Radiuses.radius8
    }

    contentItem: ColumnLayout {
        spacing: 0

        Text {
            visible: root.vTitle !== ""
            color: Colors.text
            font: Fonts.bodyBold
            text: root.vTitle
        }

        Text {
            visible: root.vText !== ""
            color: Colors.text
            opacity: 0.7
            font: Fonts.caption
            text: root.vText
        }
    }
}
