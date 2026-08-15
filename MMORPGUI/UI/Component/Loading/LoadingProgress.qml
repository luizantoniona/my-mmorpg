import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    property string vText: ""

    property color vTextColor: Colors.text
    property font vTextFont: Fonts.bodyDefault

    property color vBarColor: Colors.info
    property color vBackgroundColor: Colors.background2
    property color vBorderColor: Colors.border

    property int vRadius: Spaces.spacing8
    property int vHeight: 12
    property int vSpacing: Spaces.spacing8

    property real vProgress: 0.0
    property real vTextMaxWidth: 240

    default property alias contentData: content.data
    readonly property alias contentItem: content

    readonly property real progressValue: Math.max(0.0, Math.min(root.vProgress, 1.0))

    implicitWidth: Math.max(root.vTextMaxWidth, 200)
    implicitHeight: layout.height

    ColumnLayout {
        id: layout

        anchors.fill: parent
        spacing: root.vSpacing

        Rectangle {
            id: barBackground

            width: parent.width
            height: root.vHeight
            radius: root.vRadius
            color: root.vBackgroundColor
            border {
                width: Borders.borders1
                color: root.vBorderColor
            }
            clip: true

            Rectangle {
                id: barFill

                width: parent.width * root.progressValue
                height: parent.height
                radius: root.vRadius
                color: root.vBarColor

                Behavior on width {
                    NumberAnimation {
                        duration: 200
                    }
                }
            }
        }

        TextTitle {
            id: message

            visible: root.vText !== ""
            vText: root.vText
            vTextColor: root.vTextColor
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
            wrapMode: Text.Wrap
        }
    }

    Item {
        id: content

        anchors.fill: parent
    }
}
