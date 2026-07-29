import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGClientManagers

Item {
    id: root

    property int vStatus: ServerManager.connectionState

    PanelBase {

        ColumnLayout {
            anchors.centerIn: parent
            spacing: Spaces.spacing8

            InputFieldCustom {
                id: serverInput

                Layout.alignment: Qt.AlignHCenter

                vText: ServerManager.serverAddress
                vTitle: qsTr("Server")
                vPlaceholder: qsTr("Ex: http://127.0.0.1:8080")
            }

            ButtonBase {
                Layout.alignment: Qt.AlignHCenter

                vText: qsTr("Connect")

                onClicked: function () {
                    console.log(serverInput.vText)
                    ServerManager.connectServer(serverInput.vText)
                }
            }

            ServerStatus {
                id: serverStatus

                Layout.alignment: Qt.AlignHCenter

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
