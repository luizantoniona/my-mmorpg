import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Managers

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
        color: '#ffffff'
    }

    StackView {
        id: stack
        anchors.fill: parent
    }

    AccountManager {
        id: accountManager
    }

    Component {
        id: loginPage

        LoginPage {
            id: login

            vAccountManager: accountManager
        }
    }

    Component {
        id: accountPage

        AccountPage {
            id: account

            vAccountManager: accountManager
        }
    }

    Component {
        id: gamePage

        GamePage {
            id: game

            vAccountManager: accountManager
        }
    }

    Component.onCompleted: {
        root.updatePage("LoginPage")
    }
}
