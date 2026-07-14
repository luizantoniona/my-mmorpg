import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    PanelCustom {
        anchors.fill: parent

        vContent: RowLayout {
            spacing: Spaces.spacing8

            Text {
                text: qsTr("Server")
            }

            InputFieldCustom {
                vTitle: qsTr("")
                vPlaceholder: qsTr("")
            }

            ButtonCustom {
                vText: qsTr("Connect")
                onClicked: function () {
                    // TODO
                }
            }

            Rectangle {
                color: Colors.background4
                height: 50
                width: 50
            }
        }
    }
}
