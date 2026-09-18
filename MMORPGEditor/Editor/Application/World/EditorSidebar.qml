import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGEditorComponents

Item {
    id: root

    signal tileSelected(int type)
    signal objectSelected(int type)

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
            vText: "Tiles"
        }

        TilePalette {
            Layout.fillWidth: true
            Layout.fillHeight: true

            onTileSelected: function (type) {
                root.tileSelected(type)
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: Borders.border1
            color: Colors.border
        }

        TextSubTitle {
            Layout.fillWidth: true
            Layout.margins: Spaces.spacing8
            vText: "Objects"
        }

        ObjectPalette {
            Layout.fillWidth: true
            Layout.fillHeight: true

            onObjectSelected: function (type) {
                root.objectSelected(type)
            }
        }
    }
}
