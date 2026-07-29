import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGClientComponents

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

    Component {
        id: loginPage

        LoginPage {
            id: login
        }
    }

    Component {
        id: accountPage

        AccountPage {
            id: account
        }
    }

    Component {
        id: gamePage

        GamePage {
            id: game
        }
    }

    Component.onCompleted: {
        root.updatePage("LoginPage")
    }
}
