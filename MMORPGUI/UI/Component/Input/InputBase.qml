import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    enum InputType {
        All,
        Numeric,
        Alphanumeric
    }

    property int vEchoMode: TextInput.Normal
    property int vInputType: InputBase.All
    property string vPlaceholder: ""
    property string vText: ""
    property string vTitle: ""

    implicitHeight: 72
    implicitWidth: 280

    RegularExpressionValidator {
        id: numericValidator

        regularExpression: /[0-9]*/
    }

    RegularExpressionValidator {
        id: alphanumericValidator

        regularExpression: /[A-Za-z0-9]*/
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: Spaces.spacing8

        Text {
            Layout.fillWidth: true
            color: Colors.text
            text: root.vTitle
            visible: root.vTitle !== ""
            font: Fonts.bodyBold
            elide: Text.ElideRight
        }

        Rectangle {
            Layout.fillWidth: true
            border.color: Colors.border
            border.width: Borders.border1
            color: Colors.primaryEnabled
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
                inputMethodHints: {
                    switch (root.vInputType) {
                    case InputBase.Numeric:
                        return Qt.ImhDigitsOnly
                    default:
                        return Qt.ImhNone
                    }
                }
                validator: {
                    switch (root.vInputType) {
                    case InputBase.Numeric:
                        return numericValidator
                    case InputBase.Alphanumeric:
                        return alphanumericValidator
                    default:
                        return null
                    }
                }

                onTextChanged: root.vText = text
            }
        }
    }
}
