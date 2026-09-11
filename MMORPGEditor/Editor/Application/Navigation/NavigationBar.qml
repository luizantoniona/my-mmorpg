import QtQuick
import MMORPGUIComponents

Item {
    id: root

    property var pages: []
    property string currentPage: ""

    signal pageRequested(string page)

    height: 48

    Rectangle {
        anchors.fill: parent
        color: Colors.background2
    }

    Row {
        anchors.fill: parent

        Repeater {
            model: root.pages

            delegate: Rectangle {
                required property var modelData

                border {
                    color: Colors.border
                    width: Borders.borders1
                }
                width: 120
                height: parent.height
                color: root.currentPage === modelData.name ? Colors.primaryPressed : "transparent"

                Text {
                    anchors.centerIn: parent
                    text: modelData.label
                    font: Fonts.bodyBold
                    color: Colors.text
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: root.pageRequested(modelData.name)
                }
            }
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: Borders.border1
        color: Colors.border
    }
}
