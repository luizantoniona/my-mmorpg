import QtQuick
import QtQuick.Window
import MMORPGEditor

Window {
    width: 1280
    height: 800
    visible: true
    title: "MMORPG World Editor"

    EditorPage {
        anchors.fill: parent
    }
}
