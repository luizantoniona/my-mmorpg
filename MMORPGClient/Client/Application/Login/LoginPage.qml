import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGClientComponents
import MMORPGClientControls
import MMORPGClientManagers

Item {
    id: root

    LoginPageControl {
        id: control
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
            }
        }
    }
}
