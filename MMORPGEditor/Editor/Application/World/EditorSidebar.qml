import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGEditorComponents

Item {
    id: root

    property bool tileActive: true
    property bool objectActive: false

    property bool overlayLayer: false

    signal tileSelected(int type)
    signal objectSelected(int type)
    signal overlayLayerRequested(bool overlay)

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

        RowLayout {
            Layout.fillWidth: true
            Layout.leftMargin: Spaces.spacing8
            Layout.rightMargin: Spaces.spacing8
            spacing: Spaces.spacing8

            ButtonToggle {
                Layout.fillWidth: true
                implicitHeight: 28
                vText: "Base"
                vSelected: !root.overlayLayer
                vRadius: Radiuses.radius8

                onClicked: root.overlayLayerRequested(false)
            }

            ButtonToggle {
                Layout.fillWidth: true
                implicitHeight: 28
                vText: "Overlay"
                vSelected: root.overlayLayer
                vRadius: Radiuses.radius8

                onClicked: root.overlayLayerRequested(true)
            }
        }

        TilePalette {
            Layout.fillWidth: true
            Layout.fillHeight: true
            isActive: root.tileActive

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
            isActive: root.objectActive

            onObjectSelected: function (type) {
                root.objectSelected(type)
            }
        }
    }
}
