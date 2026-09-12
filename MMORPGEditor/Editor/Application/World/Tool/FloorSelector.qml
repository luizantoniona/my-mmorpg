import QtQuick
import MMORPGUIComponents

Item {
    id: root

    property var floors: []
    property int currentFloor: 0

    signal floorRequested(int z)

    height: 40

    Rectangle {
        anchors.fill: parent
        color: Colors.background2
    }

    Row {
        anchors.fill: parent
        anchors.margins: Spaces.spacing8
        spacing: Spaces.spacing8

        Repeater {
            model: root.floors

            delegate: ButtonToggle {
                required property int modelData

                width: 40
                height: parent.height
                vText: "Z" + modelData
                vSelected: root.currentFloor === modelData
                vRadius: Radiuses.radius8

                onClicked: root.floorRequested(modelData)
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
