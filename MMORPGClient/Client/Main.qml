import QtQuick
import QtQuick.Window
import MMORPGClientComponents

Window {
    id: window

    title: "MMORPG Client"
    visibility: Window.Windowed
    visible: true
    width: Screen.width / 1.5
    height: Screen.height / 1.5

    MainWindow {
        id: application

        anchors.fill: parent
    }
}
