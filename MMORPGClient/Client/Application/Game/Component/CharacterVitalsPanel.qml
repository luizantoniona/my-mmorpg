import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

GamePanelBase {
    id: root

    property string vCharacterName: ""
    property real vHealth: 0
    property real vMaxHealth: 1
    property real vMana: 0
    property real vMaxMana: 1
    property real vStamina: 0
    property real vMaxStamina: 1

    vTitle: root.vCharacterName
    vClosable: false
    implicitWidth: 220
    implicitHeight: column.implicitHeight + 40 + Spaces.spacing8 * 4

    ColumnLayout {
        id: column

        anchors.fill: parent
        spacing: Spaces.spacing8

        VitalBar {
            Layout.fillWidth: true
            vLabel: "HP"
            vValue: root.vHealth
            vMaxValue: root.vMaxHealth
            vBarColor: Colors.error
        }

        VitalBar {
            Layout.fillWidth: true
            vLabel: "MP"
            vValue: root.vMana
            vMaxValue: root.vMaxMana
            vBarColor: Colors.info
        }

        VitalBar {
            Layout.fillWidth: true
            vLabel: "SP"
            vValue: root.vStamina
            vMaxValue: root.vMaxStamina
            vBarColor: Colors.warning
        }
    }
}
