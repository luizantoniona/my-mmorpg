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

            delegate: ButtonToggle {
                required property var modelData

                width: 120
                height: parent.height
                vText: modelData.label
                vSelected: root.currentPage === modelData.name
                vBackgroundColor: "transparent"

                onClicked: function () {
                    root.pageRequested(modelData.name)
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
