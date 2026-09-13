import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGEditorComponents

Item {
    id: root

    readonly property var pages: [{
            "name": "WorldPage",
            "label": "World"
        }, {
            "name": "TileCreationPage",
            "label": "Tiles"
        }, {
            "name": "ObjectCreationPage",
            "label": "Objects"
        }]

    function updatePage(page) {
        stack.clear()

        switch (page) {
        case "WorldPage":
            stack.push(worldPage)
            break
        case "TileCreationPage":
            stack.push(tileCreationPage)
            break
        case "ObjectCreationPage":
            stack.push(objectCreationPage)
            break
            break
        }

        navigationBar.currentPage = page
    }

    Rectangle {
        id: background

        anchors.fill: parent
        color: Colors.background0
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        NavigationBar {
            id: navigationBar

            Layout.fillWidth: true
            pages: root.pages

            onPageRequested: function (page) {
                root.updatePage(page)
            }
        }

        StackView {
            id: stack

            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    Component {
        id: worldPage

        WorldPage {
            id: world
        }
    }

    Component {
        id: tileCreationPage

        TileCreationPage {
            id: tiles
        }
    }

    Component {
        id: objectCreationPage

        ObjectCreationPage {
            id: objects
        }
    }

    Component.onCompleted: {
        root.updatePage("WorldPage")
    }
}
