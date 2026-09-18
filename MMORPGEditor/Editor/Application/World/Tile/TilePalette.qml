import QtQuick
import MMORPGUIComponents
import MMORPGEditorControls

Item {
    id: root

    property int selectedType: -1
    property bool hasSelection: false

    signal tileSelected(int type)

    TilePaletteModel {
        id: paletteModel
    }

    GridView {
        id: grid

        anchors.fill: parent
        anchors.margins: Spaces.spacing8

        cellWidth: 48
        cellHeight: 48

        model: paletteModel

        delegate: ButtonToggle {
            required property int type
            required property string name
            required property string iconSource

            width: grid.cellWidth - 4
            height: grid.cellHeight - 4

            vSelected: root.hasSelection && root.selectedType === type

            Image {
                anchors.fill: parent
                anchors.margins: 4
                source: iconSource
                fillMode: Image.PreserveAspectFit
            }

            onClicked: {
                root.hasSelection = true
                root.selectedType = type
                root.tileSelected(type)
            }
        }
    }
}
