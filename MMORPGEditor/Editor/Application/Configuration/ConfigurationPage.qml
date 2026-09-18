import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGEditorControls

Item {
    id: root

    ConfigurationControl {
        id: configurationControl
    }

    Rectangle {
        anchors.fill: parent
        color: Colors.background0
    }

    PanelBase {
        anchors.fill: parent
        anchors.margins: Spaces.spacing8
        vHeaderType: PanelBase.PanelHeaderType.Header
        vHeaderTitle: "Configuration"

        RowLayout {
            anchors.fill: parent
            spacing: Spaces.spacing8

            ColumnLayout {
                Layout.preferredWidth: 240
                Layout.fillHeight: true
                spacing: Spaces.spacing8

                TextSubTitle {
                    vText: "Active Map"
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: Spaces.spacing8

                    Repeater {
                        model: configurationControl.mapFolders

                        ButtonToggle {
                            Layout.fillWidth: true
                            vText: modelData
                            vSelected: modelData === configurationControl.activeFolder

                            onClicked: configurationControl.selectFolder(modelData)
                        }
                    }
                }

                Text {
                    Layout.fillWidth: true
                    color: Colors.error
                    font: Fonts.bodyBold
                    wrapMode: Text.WordWrap
                    text: configurationControl.lastError
                    visible: configurationControl.lastError !== ""
                }

                Item {
                    Layout.fillHeight: true
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                spacing: Spaces.spacing8

                TextSubTitle {
                    vText: "Map Info"
                }

                Text {
                    color: Colors.text
                    font: Fonts.bodyBold
                    text: "Name: " + configurationControl.mapName
                }

                Text {
                    color: Colors.text
                    font: Fonts.bodyBold
                    text: "Floors: " + configurationControl.floorCount
                }

                TextSubTitle {
                    vText: "Resize Map"
                }

                GridLayout {
                    columns: 2
                    rowSpacing: Spaces.spacing8
                    columnSpacing: Spaces.spacing8

                    InputBase {
                        id: widthInput
                        Layout.row: 0
                        Layout.column: 0
                        Layout.preferredWidth: 120
                        vTitle: "Width (tiles)"
                        vText: String(configurationControl.mapWidth)
                    }

                    InputBase {
                        id: heightInput
                        Layout.row: 0
                        Layout.column: 1
                        Layout.preferredWidth: 120
                        vTitle: "Height (tiles)"
                        vText: String(configurationControl.mapHeight)
                    }
                }

                ButtonBase {
                    Layout.preferredWidth: 160
                    vText: "Resize"

                    onClicked: {
                        const width = parseInt(widthInput.vText, 10) || configurationControl.mapWidth;
                        const height = parseInt(heightInput.vText, 10) || configurationControl.mapHeight;
                        configurationControl.resizeMap(width, height);
                    }
                }

                Text {
                    Layout.fillWidth: true
                    color: Colors.error
                    font: Fonts.bodyBold
                    wrapMode: Text.WordWrap
                    text: configurationControl.lastError
                    visible: configurationControl.lastError !== ""
                }

                Connections {
                    target: configurationControl

                    function onActiveFolderChanged() {
                        widthInput.vText = String(configurationControl.mapWidth);
                        heightInput.vText = String(configurationControl.mapHeight);
                    }
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }
    }
}
