import QtQuick
import MMORPGUIComponents

Rectangle {
    id: root

    signal clicked

    property string vText: ""
    property bool vEnabled: true
    property bool vSelected: false
    property color vBackgroundColor: Colors.primaryEnabled
    property color vSelectedColor: Colors.primaryPressed
    property color vHoverColor: Colors.primaryHovered
    property color vBorderColor: Colors.border
    property color vTextColor: Colors.text
    property int vRadius: 0
    property int vBorderWidth: Borders.border1

    default property alias contentData: content.data
    readonly property alias contentItem: content

    readonly property bool vHovered: mouseArea.containsMouse
    readonly property bool vPressed: mouseArea.pressed

    implicitWidth: 100
    implicitHeight: 40
    opacity: root.vEnabled ? 1.0 : 0.6
    color: root.vSelected ? root.vSelectedColor : (root.vHovered ? root.vHoverColor : root.vBackgroundColor)
    border {
        width: root.vBorderWidth
        color: root.vBorderColor
    }
    radius: root.vRadius

    transitions: Transition {
        ColorAnimation {
            duration: 120
        }
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        hoverEnabled: true
        enabled: root.vEnabled
        cursorShape: Qt.PointingHandCursor

        onClicked: function () {
            root.clicked()
        }
    }

    Item {
        id: content

        anchors.fill: parent
    }

    Text {
        anchors.centerIn: parent
        text: root.vText
        color: root.vTextColor
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font: Fonts.bodyBold
        visible: root.vText !== ""
    }
}
