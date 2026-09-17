import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    property string vLabel: ""
    property real vValue: 0
    property real vMaxValue: 1
    property color vBarColor: Colors.info

    readonly property real ratio: root.vMaxValue > 0 ? Math.max(0, Math.min(1, root.vValue / root.vMaxValue)) : 0

    implicitHeight: 20

    RowLayout {
        anchors.fill: parent
        spacing: Spaces.spacing8

        Text {
            Layout.preferredWidth: 24
            color: Colors.text
            font: Fonts.caption
            text: root.vLabel
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: Radiuses.radius8
            color: Colors.background2
            border.color: Colors.border
            border.width: Borders.border1
            clip: true

            Rectangle {
                anchors {
                    left: parent.left
                    top: parent.top
                    bottom: parent.bottom
                }
                width: parent.width * root.ratio
                radius: Radiuses.radius8
                color: root.vBarColor

                Behavior on width {
                    NumberAnimation {
                        duration: 150
                    }
                }
            }

            Text {
                anchors.centerIn: parent
                color: Colors.text
                font: Fonts.caption
                text: Math.round(root.vValue) + " / " + Math.round(root.vMaxValue)
            }
        }
    }
}
