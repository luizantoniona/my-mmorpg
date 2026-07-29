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
        color: Colors.background2
        border.color: Colors.background3
        border.width: Borders.borders2
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
