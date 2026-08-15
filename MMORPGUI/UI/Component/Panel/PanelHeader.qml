import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    property string title: ""

    property alias vTitle: root.title

    Rectangle {
        id: background

        anchors.fill: parent
        color: Colors.background1
        border.color: Colors.border
        border.width: Borders.borders1
    }

    RowLayout {
        anchors {
            fill: parent
            margins: Spaces.spacing8
        }
        spacing: Spaces.spacing8

        SpacerHorizontal {}

        TextTitle {
            id: titleText

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter

            vText: root.title
        }

        SpacerHorizontal {}
    }
}
