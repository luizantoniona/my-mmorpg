import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGClientComponents
import MMORPGClientControls
import MMORPGClientManagers

Item {
    id: root

    signal loginSuccess

    LoginPageControl {
        id: control

        onLoginSucceeded: function () {
            root.loginSuccess()
        }
        onLoginFailed: function (error) {
            accountPanel.vTextError = error
        }
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: Spaces.spacing8

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Colors.background0
                // TODO: Change Rectangle by LOGO / Background / Something
            }

            ServerPanel {
                id: serverPanel

                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: ServerManager.connectionState !== ServerManager.Connected
            }

            AccountPanel {
                id: accountPanel

                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: ServerManager.connectionState === ServerManager.Connected

                onLogin: function (username, password) {
                    control.login(username, password)
                }
            }
        }
    }
}
