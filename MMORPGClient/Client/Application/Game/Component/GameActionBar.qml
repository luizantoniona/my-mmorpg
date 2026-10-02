import QtQuick
import MMORPGUIComponents

Item {
    id: root

    readonly property var vActions: [{
            "name": "inventory",
            "icon": Icons.backpack
        }, {
            "name": "equipment",
            "icon": Icons.shirt
        }, {
            "name": "skills",
            "icon": Icons.bolt
        }, {
            "name": "settings",
            "icon": Icons.adjustments
        }, {
            "name": "logout",
            "icon": Icons.logout
        }]

    readonly property real vCellSize: Math.min(48, root.width - Spaces.spacing8 * 2)

    signal actionRequested(string action)

    implicitWidth: 56
    implicitHeight: list.contentHeight + Spaces.spacing8 * 2

    Rectangle {
        anchors.fill: parent
        color: Colors.background1
        border.color: Colors.border
        border.width: Borders.border1
        radius: Radiuses.radius8
        opacity: 0.9
    }

    ListView {
        id: list

        anchors.fill: parent
        anchors.margins: Spaces.spacing8
        spacing: Spaces.spacing8
        interactive: false
        model: root.vActions

        delegate: Item {
            id: cell

            required property var modelData

            width: list.width
            height: root.vCellSize

            ButtonBase {
                anchors.horizontalCenter: parent.horizontalCenter
                width: root.vCellSize
                height: root.vCellSize
                vRadius: Radiuses.radius8

                IconBase {
                    anchors.centerIn: parent
                    vSource: cell.modelData.icon
                    vColor: Colors.text
                }

                onClicked: function () {
                    root.actionRequested(cell.modelData.name)
                }
            }
        }
    }
}
