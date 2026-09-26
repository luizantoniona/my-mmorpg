import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGEditorComponents

Item {
    id: root

    property int mode: ToolMode.Select

    readonly property var modes: [{
            "mode": ToolMode.Select,
            "icon": Icons.pointer
        }, {
            "mode": ToolMode.Paint,
            "icon": Icons.brush
        }, {
            "mode": ToolMode.Spawn,
            "icon": Icons.paw
        }]

    signal modeRequested(int mode)

    height: 40

    Row {
        anchors.fill: parent
        anchors.margins: Spaces.spacing8
        spacing: Spaces.spacing8

        Repeater {
            model: root.modes

            delegate: ButtonToggle {
                required property var modelData

                width: 40
                height: parent.height
                vSelected: root.mode === modelData.mode
                vRadius: Radiuses.radius8

                IconBase {
                    anchors.centerIn: parent
                    vSource: modelData.icon
                    vColor: Colors.text
                }

                onClicked: function () {
                    root.modeRequested(modelData.mode)
                }
            }
        }
    }
}
