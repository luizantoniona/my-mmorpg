import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property alias text: input.text
    property string labelText: ""
    property string placeholderText: ""
    property int echoMode: TextInput.Normal

    implicitWidth: 360
    implicitHeight: fieldColumn.implicitHeight

    ColumnLayout {
        id: fieldColumn
        anchors.fill: parent
        spacing: 6

        Label {
            id: label
            Layout.fillWidth: true
            text: root.labelText
            color: "#c9a227"
            font.pixelSize: 13
            font.bold: true
            visible: text.length > 0
        }

        Rectangle {
            id: frame
            Layout.fillWidth: true
            implicitHeight: 44
            radius: 6
            color: "#181f18"
            border.color: input.activeFocus ? "#1c6b4c" : "#6d5a2f"
            border.width: input.activeFocus ? 2 : 1

            TextField {
                id: input
                anchors.fill: parent
                anchors.margins: 1
                leftPadding: 12
                rightPadding: 12
                color: "#e9e5d4"
                placeholderText: root.placeholderText
                placeholderTextColor: "#8d8a7c"
                echoMode: root.echoMode
                background: null
                selectionColor: "#1c6b4c"
                selectedTextColor: "#f2f0e6"
                font.pixelSize: 14
            }
        }
    }
}
