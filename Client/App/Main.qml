import QtQuick
import QtQuick.Window

Window {
    id: window

    title: "MMORPG Client"
    visibility: Window.Windowed
    visible: true
    width: Screen.width / 1.5
    height: Screen.height / 1.5

    ApplicationWindow {
        id: application

        anchors.fill: parent
    }
}
