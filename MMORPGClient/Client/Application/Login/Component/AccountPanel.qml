import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    PanelCustom {
        anchors.fill: parent

        vContent: ColumnLayout {

            InputFieldCustom {
                vTitle: qsTr("Username")
                vPlaceholder: qsTr("")
            }

            InputFieldCustom {
                vTitle: qsTr("Password")
                vPlaceholder: qsTr("")
                vEchoMode: TextInput.Password
            }

            ButtonBase {
                vText: qsTr("Login")
                onClicked: function () {
                    /*
                        TODO: Implement login
                    */
                }
            }
        }
    }
}
