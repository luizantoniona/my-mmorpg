import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import MMORPGUIComponents

Item {
    id: root

    property string vText: ""
    property color vTextColor: Colors.text
    property font vTextFont: Fonts.bodyDefault

    property color vIndicatorColor: Colors.primary1
    property int vIndicatorSize: 48
    property int vSpacing: Spaces.spacing8

    property bool vRunning: true
    property real vTextMaxWidth: 240

    default property alias contentData: content.data
    readonly property alias contentItem: content

    implicitWidth: Math.max(vIndicatorSize, vTextMaxWidth)
    implicitHeight: column.height

    ColumnLayout {
        id: column

        anchors.fill: parent
        spacing: root.vSpacing

        BusyIndicator {
            id: indicator

            width: root.vIndicatorSize
            height: root.vIndicatorSize
            running: root.vRunning
            palette.dark: root.vIndicatorColor
            Layout.alignment: Qt.AlignHCenter
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
