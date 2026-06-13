import QtQuick
import Controls
import Managers

Item {
    id: root

    required property AccountManager vAccountManager
    required property ServerManager vServerManager

    AccountPageControl {
        id: control
    }
}
