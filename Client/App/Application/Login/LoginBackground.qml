import QtQuick

Item {
    id: root

    Rectangle {
        anchors.fill: parent
        color: "#0b1320"
    }

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: "#0a121d"
            }
            GradientStop {
                position: 0.45
                color: "#102233"
            }
            GradientStop {
                position: 1.0
                color: "#0b1320"
            }
        }
        opacity: 0.95
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: parent.height * 0.38
        color: "#060a12"
    }

    Rectangle {
        width: parent.width * 0.5
        height: parent.height * 0.3
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: -40
        color: "#0c161f"
        rotation: 5
        opacity: 0.95
    }

    Rectangle {
        width: parent.width * 0.55
        height: parent.height * 0.28
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: -60
        color: "#0f1b26"
        rotation: -6
        opacity: 0.95
    }

    Rectangle {
        width: 220
        height: 200
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height * 0.33
        color: "#111a23"
        opacity: 0.45
    }

    Repeater {
        model: 7

        Rectangle {
            required property int index

            width: 2
            height: 18 + (index % 3) * 10
            radius: 1
            color: "#c9a227"
            opacity: 0.12
            x: parent.width * 0.12 + index * (parent.width * 0.11)
            y: parent.height * 0.08 + (index % 2) * 12
        }
    }
}
