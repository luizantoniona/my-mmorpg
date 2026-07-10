import QtQuick
import MMORPGClientComponents
import MMORPGClientControls
import MMORPGClientManagers

Item {
    id: root

    required property AccountManager vAccountManager
    required property ServerManager vServerManager

    GamePageControl {
        id: control
    }
}
