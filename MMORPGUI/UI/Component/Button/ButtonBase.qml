import QtQuick
import MMORPGUIComponents

Rectangle {
    id: root

    signal clicked
    signal pressed
    signal released
    signal entered
    signal exited

    property string vText: ""
    property bool vEnabled: true
    property color vBackgroundColor: Colors.primary1
    property color vBorderColor: Colors.primary0
    property color vHoverColor: Colors.primary2
    property color vPressedColor: Colors.primary2
    property color vTextColor: Colors.text
    property int vRadius: Spaces.spacing8
    property int vBorderWidth: Borders.borders2

    default property alias contentData: content.data
    readonly property alias contentItem: content

    readonly property bool vHovered: mouseArea.containsMouse
    readonly property bool vPressed: mouseArea.pressed

    implicitWidth: 160
    implicitHeight: 40
    opacity: root.vEnabled ? 1.0 : 0.6
    color: root.vBackgroundColor
    border {
        width: root.vBorderWidth
        color: root.vBorderColor
    }
    radius: root.vRadius

    states: [
        State {
            name: "normal"
            when: root.vEnabled && !root.vHovered && !root.vPressed

            PropertyChanges {
                target: root
                color: root.vBackgroundColor
            }
        },
        State {
            name: "hover"
            when: root.vEnabled && root.vHovered && !root.vPressed

            PropertyChanges {
                target: root
                color: root.vHoverColor
            }
        },
        State {
            name: "pressed"
            when: root.vEnabled && root.vPressed

            PropertyChanges {
                target: root
                color: root.vPressedColor
            }
        },
        State {
            name: "disabled"
            when: !root.vEnabled

            PropertyChanges {
                target: root
                color: root.vBackgroundColor
                opacity: 0.6
            }
        }
    ]

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

        onEntered: function () {
            root.entered()
        }

        onExited: function () {
            root.exited()
        }

        onPressed: function () {
            root.pressed()
        }

        onReleased: function () {
            root.released()
        }

        onClicked: function () {
            root.clicked()
        }
    }

    Item {
        id: content

        anchors.fill: parent
    }

    Row {
        id: contentRow

        anchors.centerIn: parent
        spacing: 8
        visible: root.vText !== ""

        Text {
            id: label

            text: root.vText
            color: root.vTextColor
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font: Fonts.bodyBold
        }
    }
}
