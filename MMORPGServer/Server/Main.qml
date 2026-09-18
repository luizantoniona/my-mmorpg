import QtQuick
import QtQuick.Window
import MMORPGServerComponents

Window {
    id: window

    title: "MMORPG Server"
    visibility: Window.Windowed
    visible: true
    width: Screen.width / 1.5
    height: Screen.height / 1.5

    ServerWindow {
        id: application

        anchors.fill: parent
    }
}
