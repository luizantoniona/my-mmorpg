import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGEditorControls

Item {
    id: root

    readonly property bool isAnimated: textureInput.vText.toLowerCase().endsWith(".gif")
    readonly property int typeValue: parseInt(typeInput.vText, 10) || 0
    readonly property string existingName: {
        creationControl.nextType
        return creationControl.typeName(root.typeValue)
    }
    readonly property bool canSave: root.typeValue >= 1 && nameInput.vText.trim().length > 0 && textureInput.vText.length > 0

    function save(replace) {
        const frameDurationMs = root.isAnimated ? (parseInt(frameDurationInput.vText, 10) || 100) : 100
        const savedType = root.typeValue
        const ok = creationControl.createTile(savedType, nameInput.vText, textureInput.vText, tagsInput.vText, frameDurationMs, walkableToggle.vSelected, replace)

        if (ok) {
            statusText.text = replace ? "Tile replaced successfully." : "Tile created successfully."
            statusText.color = Colors.success
            nameInput.vText = ""
            textureInput.vText = ""
            tagsInput.vText = ""
            frameDurationInput.vText = "100"
            walkableToggle.vSelected = true
            typeInput.vText = String(creationControl.nextFreeType(savedType))
        } else {
            statusText.text = creationControl.lastError
            statusText.color = Colors.error
        }
    }

    TileCreationControl {
        id: creationControl
    }

    Dialog {
        id: replaceDialog

        anchors.centerIn: parent
        modal: true
        title: "Replace Tile"
        standardButtons: Dialog.Yes | Dialog.No

        Text {
            color: Colors.text
            font: Fonts.bodyBold
            text: "Type " + root.typeValue + " is used by '" + root.existingName + "'. Replace it?\nTiles already placed in the world will change."
        }

        onAccepted: root.save(true)
    }

    Component.onCompleted: typeInput.vText = String(creationControl.nextType)

    FileDialog {
        id: textureDialog
        title: "Select texture"
        nameFilters: ["Images (*.png *.gif *.webp)"]
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
        vHeaderTitle: "New Tile"

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
                    vPlaceholder: "e.g. Grass"
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: Spaces.spacing8

                    InputBase {
                        id: textureInput
                        Layout.fillWidth: true
                        vTitle: "Texture (.png, .gif or .webp)"
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

                InputBase {
                    id: tagsInput
                    Layout.fillWidth: true
                    vTitle: "Tags (comma-separated, organization only)"
                    vPlaceholder: "e.g. ground, grass"
                }

                ButtonToggle {
                    id: walkableToggle
                    Layout.preferredWidth: 160
                    vText: walkableToggle.vSelected ? "Walkable" : "Not Walkable"
                    vSelected: true

                    onClicked: walkableToggle.vSelected = !walkableToggle.vSelected
                }

                InputBase {
                    id: typeInput
                    Layout.preferredWidth: 160
                    vTitle: "Type (ID)"
                    vPlaceholder: String(creationControl.nextType)
                }

                Text {
                    Layout.fillWidth: true
                    visible: root.existingName !== "" || root.typeValue < 1
                    color: root.typeValue < 1 ? Colors.error : Colors.warning
                    font: Fonts.bodyBold
                    wrapMode: Text.WordWrap
                    text: root.typeValue < 1 ? "Type must be 1 or higher." : "Type " + root.typeValue + " is used by '" + root.existingName + "'. Saving will replace it."
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
                            tagsInput.vText = ""
                            frameDurationInput.vText = "100"
                            walkableToggle.vSelected = true
                            typeInput.vText = String(creationControl.nextType)
                            statusText.text = ""
                        }
                    }

                    ButtonBase {
                        Layout.preferredWidth: 160
                        vText: "Save Tile"
                        vEnabled: root.canSave

                        onClicked: {
                            if (root.existingName !== "") {
                                replaceDialog.open()
                            } else {
                                root.save(false)
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
