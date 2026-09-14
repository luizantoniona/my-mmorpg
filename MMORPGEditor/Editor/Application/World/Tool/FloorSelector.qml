import QtQuick
import MMORPGUIComponents

Item {
    id: root

    property var floors: []
    property int currentFloor: 0

    signal floorRequested(int z)
    signal addFloorRequested(bool above)
    signal removeFloorRequested(int z)

    height: 40

    Row {
        anchors.fill: parent
        anchors.margins: Spaces.spacing8
        spacing: Spaces.spacing8

        ButtonBase {
            width: 40
            height: parent.height

            IconBase {
                anchors.centerIn: parent
                vSource: Icons.plus
                vColor: Colors.text
            }

            onClicked: function () {
                root.addFloorRequested(false)
            }
        }

        Repeater {
            model: root.floors

            delegate: ButtonToggle {
                required property int modelData

                width: 40
                height: parent.height
                vText: "Z" + modelData
                vSelected: root.currentFloor === modelData
                vRadius: Radiuses.radius8

                onClicked: function () {
                    root.floorRequested(modelData)
                }

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
                    enabled: root.floors.length > 1
                    cursorShape: Qt.PointingHandCursor

                    onClicked: function () {
                        root.removeFloorRequested(modelData)
                    }
                }
            }
        }

        ButtonBase {
            width: 40
            height: parent.height

            IconBase {
                anchors.centerIn: parent
                vSource: Icons.plus
                vColor: Colors.text
            }

            onClicked: function () {
                root.addFloorRequested(true)
            }
        }
    }
}
