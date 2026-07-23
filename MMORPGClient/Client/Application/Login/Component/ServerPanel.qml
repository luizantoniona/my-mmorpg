import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGClientManagers

Item {
    id: root

    property int vStatus: ServerManager.connectionState

    PanelBase {
        anchors.fill: parent

        ColumnLayout {
            spacing: Spaces.spacing8

            Text {
                text: qsTr("Server")
            }

            InputFieldCustom {
                id: serverInput

                vText: ServerManager.serverAddress
                vTitle: qsTr("")
                vPlaceholder: qsTr("Ex: http://127.0.0.1:8080")
            }

            ButtonBase {
                vText: qsTr("Connect")
                onClicked: function () {
                    ServerManager.connectServer(serverInput.vText)
                }
            }

            ServerStatus {
                id: serverStatus

                vStatus: {
                    switch (ServerManager.connectionState) {
                    case ServerManager.Connected:
                        return ServerStatus.Status.Online
                    case ServerManager.Connecting:
                        return ServerStatus.Status.Connecting
                    case ServerManager.Disconnected:
                    case ServerManager.Failed:
                        return ServerStatus.Status.Offline
                    }
                }
            }
        }
    }
}
