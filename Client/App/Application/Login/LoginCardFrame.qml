import QtQuick

Item {
    id: root

    default property alias content: contentSlot.data

    Rectangle {
        id: metalFrame
        anchors.fill: parent
        radius: 10
        color: "#1a1a1a"
        border.width: 2
        border.color: "#c9a227"
    }

    Rectangle {
        id: woodPanel
        anchors.fill: parent
        anchors.margins: 8
        radius: 8
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#3b2d1b"
            }
            GradientStop {
                position: 1
                color: "#261b10"
            }
        }
        border.width: 1
        border.color: "#6d5430"
    }

    Rectangle {
        width: parent.width - 28
        height: 22
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        radius: 5
        color: "#26221a"
        border.width: 1
        border.color: "#c9a227"
        opacity: 0.85
    }

    Repeater {
        model: 4

        Rectangle {
            required property int index

            width: 10
            height: 10
            radius: 5
            color: "#c9a227"
            border.width: 1
            border.color: "#5f4c12"
            opacity: 0.95

            x: (index < 2) ? 8 : parent.width - 18
            y: (index % 2 === 0) ? 8 : parent.height - 18
        }
    }

    Item {
        id: contentSlot
        anchors.fill: parent
        anchors.margins: 24
    }
}
