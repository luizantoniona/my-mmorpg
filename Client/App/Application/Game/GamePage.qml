import QtQuick
import Controls
import Managers

Item {
    id: root

    required property AccountManager vAccountManager
    required property ServerManager vServerManager

    GamePageControl {
        id: control
    }
}
