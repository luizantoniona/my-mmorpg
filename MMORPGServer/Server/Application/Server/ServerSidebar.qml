import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    property var control: null
    property string worldName: ""
    property int worldWidth: 0
    property int worldHeight: 0
    property int currentFloor: 0

    signal characterCenterRequested(int x, int y, int z)

    Rectangle {
        anchors.fill: parent
        color: Colors.background1
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TextSubTitle {
            Layout.fillWidth: true
            Layout.margins: Spaces.spacing8
            vText: root.worldName !== "" ? root.worldName : "World"
        }

        Text {
            Layout.fillWidth: true
            Layout.leftMargin: Spaces.spacing8
            Layout.rightMargin: Spaces.spacing8
            Layout.bottomMargin: Spaces.spacing8
            color: Colors.text
            opacity: 0.7
            font: Fonts.caption
            text: root.worldWidth + " × " + root.worldHeight + " tiles · Floor " + root.currentFloor
        }

        Rectangle {
            Layout.fillWidth: true
            height: Borders.border1
            color: Colors.border
        }

        CharacterListPanel {
            Layout.fillWidth: true
            Layout.fillHeight: true
            control: root.control

            onCharacterCenterRequested: function (x, y, z) {
                root.characterCenterRequested(x, y, z)
            }
        }
    }
}
