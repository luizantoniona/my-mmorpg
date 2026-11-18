import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGClientComponents
import MMORPGClientControls
import MMORPGClientManagers

Item {
    id: root

    signal logoutSuccess
    signal enterWorldRequested(int idCharacter)
    signal createCharacterRequested

    property int selectedIdCharacter: -1
    property string selectedName: ""
    property bool hasSelection: root.selectedIdCharacter !== -1
    property bool confirmingDelete: false

    property string vTextError: ""

    AccountPageControl {
        id: control

        onLogoutSucceeded: function () {
            root.logoutSuccess()
        }
        onLogoutFailed: function (error) {
            root.vTextError = error
        }
        onCharacterRemovalFailed: function (error) {
            root.vTextError = error
        }
        onCharacterRemoved: function () {
            root.selectedIdCharacter = -1
            root.selectedName = ""
            root.confirmingDelete = false
        }
    }

    Rectangle {
        anchors.fill: parent
        color: Colors.background0
    }

    PanelBase {
        anchors.fill: parent
        anchors.margins: Spaces.spacing8
        vHeaderType: PanelBase.PanelHeaderType.Header
        vHeaderTitle: qsTr("Account")

        RowLayout {
            anchors.fill: parent
            spacing: Spaces.spacing8

            ColumnLayout {
                Layout.preferredWidth: 220
                Layout.fillWidth: false
                Layout.fillHeight: true
                spacing: Spaces.spacing8

                TextSubTitle {
                    vText: qsTr("Characters")
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: Spaces.spacing8

                    Repeater {
                        model: AccountManager.characters

                        ButtonToggle {
                            id: characterDelegate

                            required property var modelData

                            Layout.fillWidth: true

                            vText: characterDelegate.modelData.name
                            vSelected: root.selectedIdCharacter === characterDelegate.modelData.idCharacter

                            onClicked: function () {
                                root.selectedIdCharacter = characterDelegate.modelData.idCharacter
                                root.selectedName = characterDelegate.modelData.name
                                root.confirmingDelete = false
                            }
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                }

                ButtonBase {
                    Layout.fillWidth: true

                    vText: qsTr("+ New Character")

                    onClicked: function () {
                        root.createCharacterRequested()
                    }
                }

                ButtonBase {
                    Layout.fillWidth: true

                    vText: qsTr("Logout")

                    onClicked: function () {
                        control.logout()
                    }
                }

                Text {
                    Layout.fillWidth: true
                    color: Colors.error
                    font: Fonts.bodyBold
                    wrapMode: Text.WordWrap
                    text: root.vTextError
                    visible: root.vTextError !== ""
                }
            }

            PanelFrame {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: Spaces.spacing8
                    visible: !root.hasSelection

                    TextSubTitle {
                        Layout.alignment: Qt.AlignHCenter
                        vText: qsTr("Select or create a character")
                    }
                }

                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: Spaces.spacing8
                    visible: root.hasSelection

                    TextTitle {
                        Layout.alignment: Qt.AlignHCenter
                        vText: root.selectedName
                    }

                    ButtonBase {
                        Layout.alignment: Qt.AlignHCenter

                        vText: qsTr("Enter World")

                        onClicked: function () {
                            root.enterWorldRequested(root.selectedIdCharacter)
                        }
                    }

                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        spacing: Spaces.spacing8
                        visible: !root.confirmingDelete

                        ButtonBase {
                            vText: qsTr("Delete")
                            vBackgroundColor: Colors.error

                            onClicked: function () {
                                root.confirmingDelete = true
                            }
                        }
                    }

                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        spacing: Spaces.spacing8
                        visible: root.confirmingDelete

                        ButtonBase {
                            vText: qsTr("Confirm Delete")
                            vBackgroundColor: Colors.error

                            onClicked: function () {
                                control.removeCharacter(root.selectedIdCharacter)
                            }
                        }

                        ButtonBase {
                            vText: qsTr("Cancel")

                            onClicked: function () {
                                root.confirmingDelete = false
                            }
                        }
                    }
                }
            }
        }
    }
}
