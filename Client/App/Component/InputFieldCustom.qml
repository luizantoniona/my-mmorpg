import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MMORPGClient

Item {
    id: root

    property int vEchoMode: TextInput.Normal
    property string vPlaceholder: ""
    property string vText: ""
    property string vTitle: ""

    implicitHeight: 72
    implicitWidth: 280

    ColumnLayout {
        anchors.fill: parent
        spacing: Spaces.spacing8

        Text {
            color: Colors.text
            text: root.vTitle
            visible: root.vTitle !== ""
        }

        Rectangle {
            Layout.fillWidth: true
            border.color: Colors.primary0
            border.width: 1
            color: Colors.primary1
            implicitHeight: 40
            radius: 8

            TextField {
                id: textField

                anchors.fill: parent
                anchors.leftMargin: 12
                anchors.rightMargin: 12
                background: null
                color: Colors.text
                echoMode: root.vEchoMode
                placeholderText: root.vPlaceholder
                text: root.vText

                onTextChanged: root.vText = text
            }
        }
    }
}
