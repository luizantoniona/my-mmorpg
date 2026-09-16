import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    property string vCharacterName: ""
    property real vHealth: 0
    property real vMaxHealth: 1
    property real vMana: 0
    property real vMaxMana: 1
    property real vStamina: 0
    property real vMaxStamina: 1

    implicitWidth: 220
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

        Text {
            Layout.fillWidth: true
            color: Colors.text
            font: Fonts.bodyBold
            elide: Text.ElideRight
            text: root.vCharacterName
            visible: root.vCharacterName !== ""
        }

        StatBar {
            Layout.fillWidth: true
            vLabel: "HP"
            vValue: root.vHealth
            vMaxValue: root.vMaxHealth
            vBarColor: Colors.error
        }

        StatBar {
            Layout.fillWidth: true
            vLabel: "MP"
            vValue: root.vMana
            vMaxValue: root.vMaxMana
            vBarColor: Colors.info
        }

        StatBar {
            Layout.fillWidth: true
            vLabel: "SP"
            vValue: root.vStamina
            vMaxValue: root.vMaxStamina
            vBarColor: Colors.warning
        }
    }
}
