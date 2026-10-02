import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MMORPGEngine
import MMORPGUIComponents
import MMORPGClientComponents
import MMORPGClientControls
import MMORPGClientManagers

Item {
    id: root

    property int idCharacter: -1
    readonly property int currentFloor: control.characterZ
    property string vTextError: ""
    readonly property color attackFlashColor: Qt.rgba(Colors.error.r, Colors.error.g, Colors.error.b, 0.55)
    readonly property int attackFlashDurationMs: 220
    readonly property color attackWarningColor: Qt.rgba(Colors.warning.r, Colors.warning.g, Colors.warning.b, 0.45)

    signal leftWorld()

    property string characterName: {
        for (var i = 0; i < AccountManager.characters.length; i++) {
            if (AccountManager.characters[i].idCharacter === root.idCharacter) {
                return AccountManager.characters[i].name
            }
        }
        return ""
    }

    GamePageControl {
        id: control

        onWorldEntryReceived: function () {
            viewport.followEntity(root.idCharacter)
        }
        onWorldEntryFailed: function (error) {
            root.vTextError = error
        }
        onCharacterEventAttackStartReceived: function (idCharacter, x, y, z, castSeconds) {
            if (z === root.currentFloor) {
                viewport.addTileWarning(x, y, root.attackWarningColor, castSeconds * 1000)
            }
        }
        onCreatureEventAttackStartReceived: function (idCreature, x, y, z, castSeconds) {
            if (z === root.currentFloor) {
                viewport.addTileWarning(x, y, root.attackWarningColor, castSeconds * 1000)
            }
        }
        onCreatureEventAttackReceived: function (idCreature, x, y, z) {
            if (z === root.currentFloor) {
                viewport.addTileFlash(x, y, root.attackFlashColor, root.attackFlashDurationMs)
            }
        }
        onCharacterEventAttackReceived: function (idCharacter, x, y, z) {
            if (z === root.currentFloor) {
                viewport.addTileFlash(x, y, root.attackFlashColor, root.attackFlashDurationMs)
            }
        }
        onWorldLeft: function () {
            viewport.stopFollowingEntity()
            root.leftWorld()
        }
    }

    ClientRenderWorld {
        id: clientWorld
    }

    RowLayout {
        id: layoutRow

        anchors.fill: parent
        anchors.margins: Spaces.spacing8
        spacing: Spaces.spacing8

        ColumnLayout {
            id: leftDock

            readonly property real vWidth: Math.max(56, Math.min(72, layoutRow.width * 0.05))

            Layout.alignment: Qt.AlignTop
            Layout.preferredWidth: leftDock.vWidth
            Layout.minimumWidth: leftDock.vWidth
            Layout.maximumWidth: leftDock.vWidth
            spacing: Spaces.spacing8

            GameActionBar {
                Layout.fillWidth: true

                onActionRequested: function (action) {
                    switch (action) {
                    case "inventory":
                        inventoryPanel.visible = !inventoryPanel.visible
                        break
                    case "equipment":
                        equipmentPanel.visible = !equipmentPanel.visible
                        break
                    case "logout":
                        control.leaveWorld()
                        break
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: Colors.transparent
            border.color: Colors.border
            border.width: Borders.border1
            clip: true

            Viewport {
                id: viewport

                anchors.fill: parent
                anchors.margins: Borders.border1
                renderWorld: clientWorld
                activeFloor: root.currentFloor
            }
        }

        ColumnLayout {
            id: rightDock

            readonly property real vWidth: Math.max(280, Math.min(420, layoutRow.width * 0.2))

            Layout.preferredWidth: rightDock.vWidth
            Layout.minimumWidth: rightDock.vWidth
            Layout.maximumWidth: rightDock.vWidth
            Layout.fillHeight: true
            spacing: Spaces.spacing8

            CharacterVitalsPanel {
                Layout.fillWidth: true
                vCharacterName: root.characterName
                vHealth: control.health
                vMaxHealth: control.maxHealth
                vMana: control.mana
                vMaxMana: control.maxMana
                vStamina: control.stamina
                vMaxStamina: control.maxStamina
            }

            ScrollView {
                id: panelScroll

                Layout.fillWidth: true
                Layout.fillHeight: true
                contentWidth: availableWidth
                clip: true

                ColumnLayout {
                    width: panelScroll.availableWidth
                    spacing: Spaces.spacing8

                    CharacterInventoryPanel {
                        id: inventoryPanel

                        Layout.fillWidth: true
                        visible: false

                        onClosed: inventoryPanel.visible = false
                    }

                    CharacterEquipmentPanel {
                        id: equipmentPanel

                        Layout.fillWidth: true
                        visible: false

                        onClosed: equipmentPanel.visible = false
                    }
                }
            }
        }
    }

    Text {
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
        color: Colors.error
        font: Fonts.bodyBold
        text: root.vTextError
        visible: root.vTextError !== ""
    }

    Component.onCompleted: function () {
        control.loadWorld()
        clientWorld.world = control.world
        root.forceActiveFocus()
        viewport.centerCameraOnTile(control.worldWidth / 2, control.worldHeight / 2)
        control.connectToWorld(root.idCharacter)
    }

    Keys.onPressed: function (event) {
        switch (event.key) {
        case Qt.Key_W:
            control.move(0, -1)
            break
        case Qt.Key_S:
            control.move(0, 1)
            break
        case Qt.Key_A:
            control.move(-1, 0)
            break
        case Qt.Key_D:
            control.move(1, 0)
            break
        case Qt.Key_Q: {
            const dx = Math.sign(viewport.hoveredTile.x - control.characterX)
            const dy = Math.sign(viewport.hoveredTile.y - control.characterY)
            if (dx !== 0 || dy !== 0) {
                control.attack(dx, dy)
            }
            break
        }
        case Qt.Key_E:
            control.useSecondAction()
            break
        }
    }
}
