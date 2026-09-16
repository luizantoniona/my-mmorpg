import QtQuick
import MMORPGEngine
import MMORPGUIComponents
import MMORPGClientComponents
import MMORPGClientControls

Item {
    id: root

    property int idCharacter: -1
    property int currentFloor: 0
    property string vTextError: ""

    GamePageControl {
        id: control

        onWorldEntryReceived: function () {
            root.currentFloor = control.spawnFloor
            viewport.centerCameraOnTile(control.spawnX, control.spawnY)
        }
        onWorldEntryFailed: function (error) {
            root.vTextError = error
        }
        onEntityStateReceived: function (idCharacter, x, y, z, orientation) {
            clientWorld.setEntity(idCharacter, x, y, z, orientation)
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
