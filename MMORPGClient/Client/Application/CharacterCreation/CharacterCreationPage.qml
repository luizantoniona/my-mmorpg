import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGClientComponents
import MMORPGClientControls

Item {
    id: root

    signal characterCreated
    signal backRequested

    property string vTextError: ""

    CharacterCreationPageControl {
        id: control

        onCharacterCreationFailed: function (error) {
            root.vTextError = error
        }
        onCharacterCreated: function () {
            root.characterCreated()
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
        vHeaderTitle: qsTr("New Character")

        RowLayout {
            anchors.fill: parent
            spacing: Spaces.spacing8

            ColumnLayout {
                Layout.preferredWidth: 320
                Layout.fillWidth: false
                Layout.fillHeight: true
                spacing: Spaces.spacing8

                InputBase {
                    id: nameField

                    Layout.fillWidth: true

                    vTitle: qsTr("Name")
                    vPlaceholder: qsTr("")
                }

                Repeater {
                    model: control.appearanceFeatures()

                    ColumnLayout {
                        id: featureDelegate

                        required property string modelData

                        Layout.fillWidth: true
                        spacing: Spaces.spacing8

                        TextSubTitle {
                            vText: control.appearanceFeatureTitle(featureDelegate.modelData)
                        }

                        GridLayout {
                            Layout.fillWidth: true
                            columns: 2
                            columnSpacing: Spaces.spacing8
                            rowSpacing: Spaces.spacing8

                            Repeater {
                                model: control.appearanceOptions(featureDelegate.modelData, control.appearance)

                                ButtonToggle {
                                    id: optionDelegate

                                    required property var modelData

                                    Layout.fillWidth: true

                                    vText: optionDelegate.modelData.name
                                    vSelected: control.appearance[featureDelegate.modelData] === optionDelegate.modelData.id

                                    onClicked: function () {
                                        control.selectAppearanceOption(featureDelegate.modelData, optionDelegate.modelData.id)
                                    }
                                }
                            }
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                }

                ButtonBase {
                    Layout.fillWidth: true

                    vText: qsTr("Create")

                    onClicked: function () {
                        control.createCharacter(nameField.vText)
                    }
                }

                ButtonBase {
                    Layout.fillWidth: true

                    vText: qsTr("Back")

                    onClicked: function () {
                        root.backRequested()
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

                CharacterAppearancePreview {
                    anchors.fill: parent
                    anchors.margins: Spaces.spacing8

                    appearance: control.appearance
                    visible: control.hasValidAppearance
                }

                TextSubTitle {
                    anchors.centerIn: parent
                    vText: qsTr("Choose a body")
                    visible: !control.hasValidAppearance
                }
            }
        }
    }
}
