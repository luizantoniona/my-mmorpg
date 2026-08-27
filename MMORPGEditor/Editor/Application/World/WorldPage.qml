import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MMORPGEngine
import MMORPGUIComponents
import MMORPGEditorComponents
import MMORPGEditorControls

Item {
    id: root

    WorldPageControl {
        id: control
    }

    EditorRenderWorld {
        id: editorWorld
    }

    Viewport {
        id: viewport

        anchors.fill: parent
        renderWorld: editorWorld
    }

    Component.onCompleted: function () {
        control.loadWorld()
        editorWorld.world = control.world
        forceActiveFocus()
        viewport.centerCameraOnTile(16, 16)
    }

    Keys.onPressed: function (event) {
        console.log("KEY:", event.key)
        switch (event.key) {
        case Qt.Key_W:
            viewport.moveCameraByTiles(0, -1)
            break
        case Qt.Key_S:
            viewport.moveCameraByTiles(0, 1)
            break
        case Qt.Key_A:
            viewport.moveCameraByTiles(-1, 0)
            break
        case Qt.Key_D:
            viewport.moveCameraByTiles(1, 0)
            break
        }
    }
}
