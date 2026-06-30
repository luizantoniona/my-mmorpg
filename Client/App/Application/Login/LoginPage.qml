import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import MMORPGClient
import MMORPGClient.Controls
import MMORPGClient.Managers

Item {
    id: root

    required property AccountManager vAccountManager
    required property ServerManager vServerManager

    LoginPageControl {
        id: control
    }

    Item {
        anchors.fill: parent

        RowLayout {

            ColumnLayout {

                Rectangle {
                    width: 50
                    height: 50
                    color: Colors.primary
                }

                Rectangle {
                    width: 50
                    height: 50
                    color: Colors.primaryHover
                }

                Rectangle {
                    width: 50
                    height: 50
                    color: Colors.primaryPressed
                }

                Rectangle {
                    width: 50
                    height: 50
                    color: Colors.background
                }

                Rectangle {
                    width: 50
                    height: 50
                    color: Colors.surface
                }

                Rectangle {
                    width: 50
                    height: 50
                    color: Colors.accent
                }

                Rectangle {
                    width: 50
                    height: 50
                    color: Colors.accentHover
                }

                Rectangle {
                    width: 50
                    height: 50
                    color: Colors.accentPressed
                }
            }

            ColumnLayout {
                InputFieldCustom {
                    vTitle: "Usuário"
                    vPlaceholder: "Digite seu usuário"
                }

                InputFieldCustom {
                    vTitle: "Senha"
                    vPlaceholder: "Digite sua senha"
                    vEchoMode: TextInput.Password
                }

                ButtonCustom {
                    vText: "Entrar"
                    onClicked: function (){
                        console.log("Login")
                    }
                }
            }
        }
    }
}
