import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    readonly property int vWidth: parseInt(widthInput.vText, 10) || 1
    readonly property int vHeight: parseInt(heightInput.vText, 10) || 1
    readonly property real vRespawnSeconds: parseFloat(respawnInput.vText) > 0 ? parseFloat(respawnInput.vText) : 0

    function creaturesList() {
        const result = []

        for (let i = 0; i < creaturesModel.count; i++) {
            const entry = creaturesModel.get(i)
            result.push({
                "type": entry.type,
                "quantity": entry.quantity
            })
        }

        return result
    }

    function loadFrom(width, height, respawnSeconds, creatures) {
        widthInput.vText = String(width)
        heightInput.vText = String(height)
        respawnInput.vText = String(respawnSeconds)

        creaturesModel.clear()
        for (let i = 0; i < creatures.length; i++) {
            creaturesModel.append({
                "type": creatures[i].type,
                "name": creatures[i].name,
                "quantity": creatures[i].quantity
            })
        }
    }

    ListModel {
        id: creaturesModel
    }

    implicitWidth: 260
    implicitHeight: 420

    PanelFrame {
        anchors.fill: parent
        vBackgroundColor: Colors.background1
        vBorderColor: Colors.border
        vBorderWidth: Borders.border1
        vRadiusValue: Radiuses.radius8

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Spaces.spacing8
            spacing: Spaces.spacing8

            TextSubTitle {
                Layout.fillWidth: true
                vText: "Spawn Area"
            }

            GridLayout {
                Layout.fillWidth: true
                columns: 2
                columnSpacing: Spaces.spacing8

                InputBase {
                    id: widthInput

                    Layout.fillWidth: true
                    vTitle: "Width"
                    vText: "4"
                }

                InputBase {
                    id: heightInput

                    Layout.fillWidth: true
                    vTitle: "Height"
                    vText: "4"
                }

                InputBase {
                    id: respawnInput

                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    vTitle: "Respawn (seconds)"
                    vPlaceholder: "Required"
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: Borders.border1
                color: Colors.border
            }

            TextSubTitle {
                Layout.fillWidth: true
                vText: "Creatures"
            }

            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                spacing: Spaces.spacing8
                model: creaturesModel

                delegate: RowLayout {
                    id: creatureRow

                    required property int index
                    required property string name
                    required property int quantity

                    width: ListView.view.width
                    spacing: Spaces.spacing8

                    Text {
                        Layout.fillWidth: true
                        color: Colors.text
                        font: Fonts.bodyDefault
                        text: creatureRow.name
                        elide: Text.ElideRight
                    }

                    InputBase {
                        Layout.preferredWidth: 60
                        Layout.preferredHeight: 40
                        vText: String(creatureRow.quantity)

                        onVTextChanged: {
                            const parsed = parseInt(vText, 10)
                            if (!isNaN(parsed) && parsed > 0) {
                                creaturesModel.setProperty(creatureRow.index, "quantity", parsed)
                            }
                        }
                    }

                    ButtonBase {
                        Layout.preferredWidth: 24
                        Layout.preferredHeight: 24
                        vBackgroundColor: Colors.background1
                        vHoverColor: Colors.background2
                        vRadius: Radiuses.radius8

                        IconBase {
                            anchors.centerIn: parent
                            vSize: 14
                            vSource: Icons.trash
                            vColor: Colors.text
                        }

                        onClicked: creaturesModel.remove(creatureRow.index)
                    }
                }
            }

            ButtonBase {
                Layout.fillWidth: true
                Layout.preferredHeight: 32
                vText: "Add Creature"
                vRadius: Radiuses.radius8

                onClicked: creatureTypePicker.open()
            }
        }
    }

    CreatureTypePicker {
        id: creatureTypePicker

        onCreatureTypeSelected: function (type, name) {
            creaturesModel.append({
                "type": type,
                "name": name,
                "quantity": 1
            })
        }
    }
}
