import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MMORPGClient
import MMORPGClient.Controls
import MMORPGClient.Managers

Item {
    id: root

    function updatePage(page) {
        stack.clear()

        switch (page) {
        case "LoginPage":
            stack.push(loginPage)
            break
        case "AccountPage":
            stack.push(accountPage)
            break
        case "GamePage":
            stack.push(gamePage)
            break
        }
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: Colors.background0
    }

    StackView {
        id: stack
        anchors.fill: parent
    }

    AccountManager {
        id: accountManager
    }

    ServerManager {
        id: serverManager
    }

    Component {
        id: loginPage

        LoginPage {
            id: login

            vAccountManager: accountManager
            vServerManager: serverManager
        }
    }

    Component {
        id: accountPage

        AccountPage {
            id: account

            vAccountManager: accountManager
            vServerManager: serverManager
        }
    }

    Component {
        id: gamePage

        GamePage {
            id: game

            vAccountManager: accountManager
            vServerManager: serverManager
        }
    }

    Component.onCompleted: {
        root.updatePage("LoginPage")
    }
}
