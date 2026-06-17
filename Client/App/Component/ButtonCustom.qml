import QtQuick
import QtQuick.Controls

Button {
    id: root

    property color primaryColor: "#0b3d2e"
    property color hoverColor: "#1c6b4c"
    property color borderColor: "#c9a227"
    property color textColor: "#efe7c2"
    property bool secondary: false

    implicitWidth: 220
    implicitHeight: 44
    hoverEnabled: true

    contentItem: Text {
        text: root.text
        color: root.secondary ? "#c9a227" : root.textColor
        font.pixelSize: 15
        font.bold: !root.secondary
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        radius: 6
        border.width: root.secondary ? 1 : 2
        border.color: root.borderColor
        color: {
            if (root.secondary)
                return root.down ? "#2a2516" : root.hovered ? "#1f1a0f" : "#17130c";

            return root.down ? root.primaryColor : root.hovered ? root.hoverColor : root.primaryColor;
        }
    }
}
