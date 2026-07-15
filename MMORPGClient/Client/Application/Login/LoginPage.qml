import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGClientComponents
import MMORPGClientControls

Item {
    id: root

    LoginPageControl {
        id: control

        onConnectionSuccess: function () {
            console.log("Conectado com sucesso ao servidor:")
            // TODO: Show success or create status circle
        }

        onConnectionFailed: function (error) {
            console.error("Falha ao conectar no servidor:")
            // TODO: Show error or create status circle
        }
    }

    // TODO: Add background image?
    ColumnLayout {
        anchors.fill: parent

        ServerPanel {
            Layout.fillWidth: true
            Layout.preferredHeight: 200

            vServerText: control.serverAddress

            onConnectClicked: function (server) {
                control.connectServer(server)
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: Spaces.spacing8

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                // TODO: LOGO / Background / Something
            }

            AccountPanel {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
