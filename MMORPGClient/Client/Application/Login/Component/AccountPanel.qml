import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    PanelBase {

        ColumnLayout {
            anchors.centerIn: parent
            spacing: Spaces.spacing8

            InputFieldCustom {
                Layout.alignment: Qt.AlignHCenter

                vTitle: qsTr("Username")
                vPlaceholder: qsTr("")
            }

            InputFieldCustom {
                Layout.alignment: Qt.AlignHCenter

                vTitle: qsTr("Password")
                vPlaceholder: qsTr("")
                vEchoMode: TextInput.Password
            }

            ButtonBase {
                Layout.alignment: Qt.AlignHCenter

                vText: qsTr("Login")
                onClicked: function () {
                    /*
                        TODO: Implement login
                    */
                }
            }

            ButtonBase {
                Layout.alignment: Qt.AlignHCenter

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
