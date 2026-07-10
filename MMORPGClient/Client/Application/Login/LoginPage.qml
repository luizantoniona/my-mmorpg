import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import MMORPGClientComponents
import MMORPGClientControls
import MMORPGClientManagers

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

            // PanelCustom {
            //     width: 400
            //     height: 300

            //     ColumnLayout {
            //         anchors.centerIn: parent

            //         InputFieldCustom {
            //             vTitle: "Usuário"
            //             vPlaceholder: "Digite seu usuário"
            //         }

            //         InputFieldCustom {
            //             vTitle: "Senha"
            //             vPlaceholder: "Digite sua senha"
            //             vEchoMode: TextInput.Password
            //         }

            //         ButtonCustom {
            //             vText: "Entrar"
            //             onClicked: function (){
            //                 console.log("Login")
            //             }
            //         }
            //     }
            // }
        }
    }
}
