import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGEditorControls

Item {
    id: root

    readonly property bool isAnimated: textureInput.vText.toLowerCase().endsWith(".gif")
    readonly property bool canSave: nameInput.vText.trim().length > 0 && textureInput.vText.length > 0

    ObjectCreationControl {
        id: creationControl
    }

    FileDialog {
        id: textureDialog
        title: "Select texture"
        nameFilters: ["Images (*.png *.gif)"]
        onAccepted: textureInput.vText = selectedFile.toString()
    }

    Rectangle {
        anchors.fill: parent
        color: Colors.background0
    }

    PanelBase {
        anchors.fill: parent
        anchors.margins: Spaces.spacing8
        vHeaderType: PanelBase.PanelHeaderType.Header
        vHeaderTitle: "New Object"

        RowLayout {
            anchors.fill: parent
            spacing: Spaces.spacing8

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: Spaces.spacing8

                InputBase {
                    id: nameInput
                    Layout.fillWidth: true
                    vTitle: "Name"
                    vPlaceholder: "e.g. StoneWall"
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: Spaces.spacing8

                    InputBase {
                        id: textureInput
                        Layout.fillWidth: true
                        vTitle: "Texture (.png or .gif)"
                        vPlaceholder: "No texture selected"
                    }

                    ButtonBase {
                        Layout.alignment: Qt.AlignBottom
                        vText: "Browse..."

                        onClicked: textureDialog.open()
                    }
                }

                InputBase {
                    id: frameDurationInput
                    visible: root.isAnimated
                    Layout.preferredWidth: 160
                    vTitle: "Frame duration (ms)"
                    vPlaceholder: "100"
                    vText: "100"
                }

                GridLayout {
                    Layout.fillWidth: true
                    columns: 2
                    rowSpacing: Spaces.spacing8
                    columnSpacing: Spaces.spacing8

                    InputBase {
                        id: widthInput
                        Layout.row: 0
                        Layout.column: 0
                        Layout.preferredWidth: 120
                        vTitle: "Width (tiles)"
                        vPlaceholder: "1"
                        vText: "1"
                    }

                    InputBase {
                        id: heightInput
                        Layout.row: 0
                        Layout.column: 1
                        Layout.preferredWidth: 120
                        vTitle: "Height (tiles)"
                        vPlaceholder: "1"
                        vText: "1"
                    }
                }

                Text {
                    color: Colors.text
                    font: Fonts.bodyBold
                    text: "Type (auto): " + creationControl.nextType
                }

                Item {
                    Layout.fillHeight: true
                }

                Text {
                    id: statusText
                    Layout.fillWidth: true
                    font: Fonts.bodyBold
                    wrapMode: Text.WordWrap
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: Spaces.spacing8

                    ButtonBase {
                        vText: "Clear"
                        vBackgroundColor: Colors.background1
                        vHoverColor: Colors.background2

                        onClicked: {
                            nameInput.vText = ""
                            textureInput.vText = ""
                            frameDurationInput.vText = "100"
                            widthInput.vText = "1"
                            heightInput.vText = "1"
                            statusText.text = ""
                        }
                    }

                    ButtonBase {
                        Layout.preferredWidth: 160
                        vText: "Save Object"
                        vEnabled: root.canSave

                        onClicked: {
                            const frameDurationMs = root.isAnimated ? (parseInt(frameDurationInput.vText, 10) || 100) : 100
                            const width = parseInt(widthInput.vText, 10) || 1
                            const height = parseInt(heightInput.vText, 10) || 1
                            const ok = creationControl.createObject(nameInput.vText, textureInput.vText, width, height, frameDurationMs)

                            if (ok) {
                                statusText.text = "Object created successfully."
                                statusText.color = Colors.success
                                nameInput.vText = ""
                                textureInput.vText = ""
                                frameDurationInput.vText = "100"
                                widthInput.vText = "1"
                                heightInput.vText = "1"
                            } else {
                                statusText.text = creationControl.lastError
                                statusText.color = Colors.error
                            }
                        }
                    }
                }
            }

            ColumnLayout {
                Layout.preferredWidth: 160
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                spacing: Spaces.spacing8

                Text {
                    color: Colors.text
                    font: Fonts.bodyBold
                    text: "Preview"
                }

                Rectangle {
                    Layout.preferredWidth: 128
                    Layout.preferredHeight: 128
                    color: Colors.background1
                    border.width: Borders.border1
                    border.color: Colors.border
                    radius: Radiuses.radius8

                    AnimatedImage {
                        anchors.fill: parent
                        anchors.margins: Spaces.spacing8
                        fillMode: Image.PreserveAspectFit
                        source: textureInput.vText
                        visible: textureInput.vText !== ""
                    }

                    Text {
                        anchors.centerIn: parent
                        color: Colors.text
                        opacity: 0.5
                        font: Fonts.bodyBold
                        text: "No texture"
                        visible: textureInput.vText === ""
                    }
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }
    }
}
