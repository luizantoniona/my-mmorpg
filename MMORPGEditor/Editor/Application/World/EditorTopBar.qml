import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGEditorComponents

Item {
    id: root

    property int mode: ToolMode.Select
    property var floors: []
    property int currentFloor: 0

    signal modeRequested(int mode)
    signal floorRequested(int z)
    signal saveRequested

    height: modeBar.height

    RowLayout {
        anchors.fill: parent
        spacing: 0

        ToolSelector {
            id: modeBar

            // Layout.fillHeight: true
            Layout.fillWidth: true
            mode: root.mode

            onModeRequested: function (mode) {
                root.modeRequested(mode)
            }
        }

        SpacerHorizontal {}

        FloorSelector {
            id: floorSelector

            Layout.fillHeight: true
            Layout.fillWidth: true
            floors: root.floors
            currentFloor: root.currentFloor

            onFloorRequested: function (z) {
                root.floorRequested(z)
            }
        }

        SpacerHorizontal {}

        ButtonBase {
            id: saveButton

            Layout.fillHeight: true
            width: 24
            anchors.margins: Spaces.spacing8

            IconBase {
                anchors.centerIn: parent
                vSource: Icons.save
                vColor: Colors.text
            }

            onClicked: function () {
                root.saveRequested()
            }
        }
    }
}
