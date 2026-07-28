import QtQuick
import QtQuick.Controls
import MMORPGUIComponents
import MMORPGEditorControls

Rectangle {
    id: root

    focus: true

    color: Colors.background0

    readonly property int tileSize: 8

    Keys.onPressed: function(event) {
        if (event.key === Qt.Key_W) {
            control.moveBy(0, 1)
            event.accepted = true
        } else if (event.key === Qt.Key_S) {
            control.moveBy(0, -1)
            event.accepted = true
        } else if (event.key === Qt.Key_A) {
            control.moveBy(-1, 0)
            event.accepted = true
        } else if (event.key === Qt.Key_D) {
            control.moveBy(1, 0)
            event.accepted = true
        }
    }

    EditorPageControl {
        id: control
    }

    Column {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Rectangle {
            width: parent.width
            height: 92
            radius: 8
            color: "#1f2937"

            Row {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 12

                ComboBox {
                    id: floorBox

                    model: control.floors
                    textRole: "name"

                    onActivated: {
                        control.currentFloor = model[index].z
                    }
                }


                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#e5e7eb"
                    text: "Mundo: " + control.worldRoot
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#9ca3af"
                    text: "WASD navega entre chunks"
                }
            }
        }

        Text {
            color: "#9ca3af"
            text: control.statusMessage
        }

        Flickable {
            id: worldViewport
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            width: parent.width
            height: parent.height - 140
            contentWidth: chunkGrid.width
            contentHeight: chunkGrid.height

            Grid {
                columns: control.worldWidth

                Repeater {
                    model: control.currentTiles

                    Rectangle {
                        required property var modelData

                        width: root.tileSize
                        height: root.tileSize

                        color: modelData.walkable ? "#3b82f6" : "#111827"

                        Image {
                            anchors.fill: parent
                            source: modelData.texture
                            smooth: false
                        }
                    }
                }
            }
        }
    }
}
