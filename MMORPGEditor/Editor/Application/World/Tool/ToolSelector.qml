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
        }]

    signal modeRequested(int mode)

    Rectangle {
        anchors.fill: parent
        color: Colors.background2
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: Spaces.spacing8
        spacing: Spaces.spacing8

        Repeater {
            model: root.modes

            delegate: ButtonToggle {
                required property var modelData

                width: 24
                height: 24
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
