import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    property alias vServerText: serverInput.vText

    signal connectClicked(string server)

    PanelCustom {
        anchors.fill: parent

        vContent: RowLayout {
            spacing: Spaces.spacing8

            Text {
                text: qsTr("Server")
            }

            InputFieldCustom {
                id: serverInput

                vTitle: qsTr("")
                vPlaceholder: qsTr("Ex: http://127.0.0.1:8080")
            }

            ButtonCustom {
                vText: qsTr("Connect")
                onClicked: function () {
                    connectClicked(serverInput.vText)
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
