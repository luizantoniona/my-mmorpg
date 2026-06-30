import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MMORPGClient

Item {
    id: root

    property string vTitle: ""
    property string vText: ""
    property string vPlaceholder: ""
    property int vEchoMode: TextInput.Normal

    implicitWidth: 280
    implicitHeight: 72

    ColumnLayout {
        anchors.fill: parent
        spacing: 4

        Text {
            text: root.vTitle
            color: Colors.textOnBackground
            visible: root.vTitle !== ""
        }

        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 40
            radius: 6
            color: Colors.surface
            border.width: 1
            border.color: Colors.outline

            TextField {
                id: textField

                anchors.fill: parent
                anchors.leftMargin: 12
                anchors.rightMargin: 12
                text: root.vText
                placeholderText: root.vPlaceholder
                echoMode: root.vEchoMode
                background: null
                color: Colors.textOnSurface
                onTextChanged: root.vText = text
            }
        }
    }
}
