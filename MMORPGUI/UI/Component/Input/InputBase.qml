import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MMORPGUIComponents

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
            font: Fonts.bodyBold
        }

        Rectangle {
            Layout.fillWidth: true
            border.color: Colors.primary0
            border.width: Borders.borders2
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
                font: Fonts.bodyBold

                onTextChanged: root.vText = text
            }
        }
    }
}
