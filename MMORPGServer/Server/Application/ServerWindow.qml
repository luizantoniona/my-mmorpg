import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGServerComponents

Item {
    id: root

    function updatePage(page) {
        stack.clear()

        switch (page) {
        case "ServerPage":
            stack.push(serverPage)
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
        id: serverPage

        ServerPage {
            id: server
        }
    }

    Component.onCompleted: {
        root.updatePage("ServerPage")
    }
}
