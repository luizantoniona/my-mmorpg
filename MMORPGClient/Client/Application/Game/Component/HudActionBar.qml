import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    readonly property var vActions: [{
            "name": "inventory",
            "icon": Icons.backpack
        }, {
            "name": "skills",
            "icon": Icons.bolt
        }, {
            "name": "settings",
            "icon": Icons.adjustments
        }]

    signal actionRequested(string action)

    implicitWidth: 56
    implicitHeight: column.implicitHeight + Spaces.spacing8 * 2

    Rectangle {
        anchors.fill: parent
        color: Colors.background1
        border.color: Colors.border
        border.width: Borders.border1
        radius: Radiuses.radius8
        opacity: 0.9
    }

    ColumnLayout {
        id: column

        anchors.fill: parent
        anchors.margins: Spaces.spacing8
        spacing: Spaces.spacing8

        Repeater {
            model: root.vActions

            delegate: ButtonBase {
                required property var modelData

                Layout.fillWidth: true
                Layout.preferredHeight: width
                vRadius: Radiuses.radius8

                IconBase {
                    anchors.centerIn: parent
                    vSource: modelData.icon
                    vColor: Colors.text
                }

                onClicked: function () {
                    root.actionRequested(modelData.name)
                }
            }
        }
    }
}
