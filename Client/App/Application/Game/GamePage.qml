import QtQuick
import MMORPGClient
import MMORPGClient.Controls
import MMORPGClient.Managers

Item {
    id: root

    required property AccountManager vAccountManager
    required property ServerManager vServerManager

    GamePageControl {
        id: control
    }
}
