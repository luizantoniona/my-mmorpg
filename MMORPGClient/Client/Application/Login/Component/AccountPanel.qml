import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    PanelBase {
        anchors.fill: parent

        ColumnLayout {

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

            ButtonBase {
                vText: qsTr("Disconnect")
                onClicked: function () {
                    /*
                        TODO: Implement disconnect for server change
                    */
                }
            }
        }
    }
}
