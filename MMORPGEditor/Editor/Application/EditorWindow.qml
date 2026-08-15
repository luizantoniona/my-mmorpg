import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGEditorComponents

Item {
    id: root

    function updatePage(page) {
        stack.clear()

        switch (page) {
        case "WorldPage":
            stack.push(worldPage)
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
        id: worldPage

        WorldPage {
            id: world
        }
    }

    Component.onCompleted: {
        root.updatePage("WorldPage")
    }
}
