import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGClientManagers

Item {
    id: root

    signal login(string username, string password)

    property string vTextError: ""

    PanelBase {
        ColumnLayout {
            anchors.centerIn: parent
            spacing: Spaces.spacing8

            InputBase {
                id: usernameField

                Layout.alignment: Qt.AlignHCenter

                vTitle: qsTr("Username")
                vPlaceholder: qsTr("")
            }

            InputBase {
                id: passwordField

                Layout.alignment: Qt.AlignHCenter

                vTitle: qsTr("Password")
                vPlaceholder: qsTr("")
                vEchoMode: TextInput.Password
            }

            ButtonBase {
                Layout.alignment: Qt.AlignHCenter

                vText: qsTr("Login")
                onClicked: function () {
                    root.login(usernameField.vText, passwordField.vText)
                }
            }

            ButtonBase {
                Layout.alignment: Qt.AlignHCenter

                vText: qsTr("Disconnect")
                onClicked: function () {
                    ServerManager.disconnectServer()
                }
            }

            TextTitle {
                visible: root.vTextError.length > 0
                vText: root.vTextError
                vTextColor: Colors.error
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }
}
