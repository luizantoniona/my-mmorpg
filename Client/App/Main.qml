import QtQuick
import QtQuick.Window

Window {
    id: window

    title: "MMORPG Client"
    visibility: Window.Windowed
    visible: true

    width: 1024
    height: 768

    ApplicationWindow {
        id: application

        anchors.fill: parent
    }
}
