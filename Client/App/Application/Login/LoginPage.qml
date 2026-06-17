import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Controls
import Managers

Item {
    id: root

    required property AccountManager vAccountManager
    required property ServerManager vServerManager

    LoginPageControl {
        id: control
    }

    LoginBackground {
        anchors.fill: parent
    }

    Rectangle {
        anchors.fill: parent
        color: "#000000"
        opacity: 0.28
    }

    Item {
        anchors.fill: parent

        LoginCardFrame {
            id: loginCard
            width: Math.min(460, parent.width * 0.85)
            height: 500
            anchors.centerIn: parent

            ColumnLayout {
                anchors.fill: parent
                spacing: 14

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 10
                }

                Label {
                    Layout.fillWidth: true
                    text: "MyMMORPG"
                    color: "#c9a227"
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 38
                    font.bold: true
                }

                Label {
                    Layout.fillWidth: true
                    text: "A realm awaits"
                    color: "#d4c791"
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 14
                    opacity: 0.9
                }

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 8
                }

                InputFieldCustom {
                    id: userField
                    Layout.fillWidth: true
                    labelText: "Usuario"
                    placeholderText: "Digite seu usuario"
                }

                InputFieldCustom {
                    id: passwordField
                    Layout.fillWidth: true
                    labelText: "Senha"
                    placeholderText: "Digite sua senha"
                    echoMode: TextInput.Password
                }

                InputFieldCustom {
                    id: serverField
                    Layout.fillWidth: true
                    labelText: "Servidor"
                    placeholderText: "127.0.0.1:8080"
                }

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 4
                }

                ButtonCustom {
                    id: loginButton
                    Layout.fillWidth: true
                    text: "Entrar"
                    primaryColor: "#0b3d2e"
                    hoverColor: "#1c6b4c"
                    borderColor: "#c9a227"
                }

                ButtonCustom {
                    id: createAccountButton
                    Layout.fillWidth: true
                    text: "Criar conta"
                    secondary: true
                    borderColor: "#6d5430"
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
    }
}
