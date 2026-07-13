import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import MMORPGUIComponents
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

    ColumnLayout {
        spacing: Spaces.spacing8
        anchors.fill: parent

        PanelCustom {
            Layout.fillWidth: true
            Layout.preferredHeight: 200

            InputFieldCustom {
                vTitle: qsTr("Server")
                vPlaceholder: qsTr("")
            }
        }

        SpacerVertical {}

        RowLayout {
            spacing: Spaces.spacing8

            ColumnLayout {
                Rectangle {
                    color: Colors.background4
                    height: Screen.height * 0.5
                    width: Screen.width * 0.5
                }
            }

            SpacerHorizontal {}

            ColumnLayout {
                PanelCustom {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ColumnLayout {
                        anchors.fill: parent

                        InputFieldCustom {
                            vTitle: qsTr("Username")
                            vPlaceholder: qsTr("")
                        }

                        InputFieldCustom {
                            vTitle: qsTr("Password")
                            vPlaceholder: qsTr("")
                            vEchoMode: TextInput.Password
                        }

                        ButtonCustom {
                            vText: "Entrar"
                            onClicked: function () {
                                vServerManager.serverAddress = "";
                            }
                        }
                    }
                }
            }
        }

        SpacerVertical {}
    }
}
