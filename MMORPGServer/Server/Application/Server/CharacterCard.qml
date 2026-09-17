import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    property int idCharacter: 0
    property string name: ""
    property int tileX: 0
    property int tileY: 0
    property int tileZ: 0

    signal centerRequested
    signal disconnectRequested

    implicitHeight: 64

    Rectangle {
        anchors.fill: parent
        color: mouseArea.containsMouse ? Colors.primaryHovered : Colors.background2
        border.color: Colors.border
        border.width: Borders.border1
        radius: Radiuses.radius8

        Behavior on color {
            ColorAnimation {
                duration: 120
            }
        }
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onClicked: function () {
            root.centerRequested()
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: Spaces.spacing8
        spacing: Spaces.spacing8

        Rectangle {
            Layout.preferredWidth: 32
            Layout.preferredHeight: 32
            Layout.alignment: Qt.AlignVCenter
            radius: 16
            color: Colors.info

            Text {
                anchors.centerIn: parent
                text: root.name.length > 0 ? root.name.charAt(0).toUpperCase() : "?"
                color: Colors.text
                font: Fonts.bodyBold
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 2

            Text {
                Layout.fillWidth: true
                color: Colors.text
                font: Fonts.bodyBold
                elide: Text.ElideRight
                text: root.name + " · #" + root.idCharacter
            }

            Text {
                Layout.fillWidth: true
                color: Colors.text
                opacity: 0.7
                font: Fonts.caption
                text: "X" + root.tileX + " Y" + root.tileY + " Z" + root.tileZ
            }
        }

        ButtonBase {
            Layout.preferredWidth: 28
            Layout.preferredHeight: 28
            Layout.alignment: Qt.AlignVCenter
            vBackgroundColor: Colors.background2
            vHoverColor: Colors.error
            vBorderWidth: 0

            IconBase {
                anchors.centerIn: parent
                vSource: Icons.trash
                vColor: Colors.text
            }

            onClicked: function () {
                root.disconnectRequested()
            }
        }
    }
}
