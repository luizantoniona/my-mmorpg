import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    property var control: null
    property var characters: []

    signal characterCenterRequested(int x, int y, int z)

    Timer {
        interval: 1000
        running: root.visible
        repeat: true
        triggeredOnStart: true

        onTriggered: function () {
            if (root.control) {
                root.characters = root.control.connectedCharacters()
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: Spaces.spacing8

        TextSubTitle {
            Layout.fillWidth: true
            Layout.margins: Spaces.spacing8
            vText: "Connected (" + root.characters.length + ")"
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: Spaces.spacing8
            Layout.rightMargin: Spaces.spacing8
            Layout.bottomMargin: Spaces.spacing8
            clip: true
            spacing: Spaces.spacing8
            model: root.characters

            delegate: CharacterCard {
                width: ListView.view.width
                idCharacter: modelData.idCharacter
                name: modelData.name
                tileX: modelData.x
                tileY: modelData.y
                tileZ: modelData.z

                onCenterRequested: function () {
                    root.characterCenterRequested(modelData.x, modelData.y, modelData.z)
                }

                onDisconnectRequested: function () {
                    disconnectDialog.pendingId = modelData.idCharacter
                    disconnectDialog.pendingName = modelData.name
                    disconnectDialog.open()
                }
            }
        }
    }

    Dialog {
        id: disconnectDialog

        property int pendingId: 0
        property string pendingName: ""

        anchors.centerIn: parent
        modal: true
        title: "Disconnect Character"
        standardButtons: Dialog.Yes | Dialog.No

        Text {
            color: Colors.text
            font: Fonts.bodyBold
            text: "Disconnect " + disconnectDialog.pendingName + " (#" + disconnectDialog.pendingId + ")?"
        }

        onAccepted: function () {
            if (root.control) {
                root.control.disconnectCharacter(disconnectDialog.pendingId)
            }
        }
    }
}
