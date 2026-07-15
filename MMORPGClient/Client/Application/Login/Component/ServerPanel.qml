import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    property alias serverText: serverInput.vText

    signal connectClicked(string server)

    function handleConnectionSuccess() {
        serverStatus.vStatus = ServerStatus.Status.Online
    }

    function handleConnectionConnecting() {
        serverStatus.vStatus = ServerStatus.Status.Connecting
    }

    function handleConnectionFailed() {
        serverStatus.vStatus = ServerStatus.Status.Offline
    }

    PanelCustom {
        anchors.fill: parent

        vContent: RowLayout {
            spacing: Spaces.spacing8

            Text {
                text: qsTr("Server")
            }

            InputFieldCustom {
                id: serverInput

                vTitle: qsTr("")
                vPlaceholder: qsTr("Ex: http://127.0.0.1:8080")
            }

            ButtonCustom {
                vText: qsTr("Connect")
                onClicked: function () {
                    connectClicked(serverInput.vText)
                }
            }

            ServerStatus {
                id: serverStatus

                vStatus: ServerStatus.Status.Offline
            }
        }
    }
}
