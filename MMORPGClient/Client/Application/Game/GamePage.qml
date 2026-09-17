import QtQuick
import MMORPGEngine
import MMORPGUIComponents
import MMORPGClientComponents
import MMORPGClientControls
import MMORPGClientManagers

Item {
    id: root

    property int idCharacter: -1
    property int currentFloor: 0
    property string vTextError: ""

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
            root.currentFloor = control.spawnFloor
            viewport.centerCameraOnTile(control.spawnX, control.spawnY)
        }
        onWorldEntryFailed: function (error) {
            root.vTextError = error
        }
        onEntityStateReceived: function (idCharacter, x, y, z) {
            clientWorld.setEntity(idCharacter, x, y, z)
        }
        onEntityLeftReceived: function (idCharacter) {
            clientWorld.removeEntity(idCharacter)
        }
    }

    ClientRenderWorld {
        id: clientWorld
    }

    Viewport {
        id: viewport

        anchors.fill: parent
        renderWorld: clientWorld
        activeFloor: root.currentFloor
    }

    CharacterHud {
        anchors {
            top: parent.top
            left: parent.left
            margins: Spaces.spacing8
        }
        vCharacterName: root.characterName
        vHealth: control.health
        vMaxHealth: control.maxHealth
        vMana: control.mana
        vMaxMana: control.maxMana
        vStamina: control.stamina
        vMaxStamina: control.maxStamina
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
        }
    }
}
