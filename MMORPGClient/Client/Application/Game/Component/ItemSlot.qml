import QtQuick
import MMORPGUIComponents

Rectangle {
    id: root

    property string vLabel: ""
    property string vItemName: ""
    property int vAmount: 0

    readonly property bool vIsEmpty: root.vItemName === ""

    color: Colors.background2
    border.color: root.vIsEmpty ? Colors.border : Colors.info
    border.width: Borders.border1
    radius: Radiuses.radius8

    states: State {
        name: "hovered"
        when: hoverHandler.hovered && !root.vIsEmpty

        PropertyChanges {
            target: root
            color: Colors.primaryHovered
        }
    }

    transitions: Transition {
        ColorAnimation {
            duration: 120
        }
    }

    HoverHandler {
        id: hoverHandler
    }

    TooltipBase {
        visible: hoverHandler.hovered && !root.vIsEmpty
        vTitle: root.vItemName
        vText: root.vLabel
    }

    Text {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: Spaces.spacing8
        }
        visible: root.vLabel !== ""
        color: Colors.text
        opacity: 0.6
        font: Fonts.caption
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
        text: root.vLabel
    }

    Text {
        anchors {
            bottom: parent.bottom
            right: parent.right
            margins: Spaces.spacing8
        }
        visible: root.vAmount > 1
        color: Colors.text
        font: Fonts.caption
        text: "x" + root.vAmount
    }
}
