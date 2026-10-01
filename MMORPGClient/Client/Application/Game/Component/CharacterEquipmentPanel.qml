import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGClientControls

GamePanelBase {
    id: root

    CharacterEquipmentPanelControl {
        id: control
    }

    readonly property var vSlots: [{
            "slot": "HEAD",
            "label": qsTr("Head")
        }, {
            "slot": "NECK",
            "label": qsTr("Neck")
        }, {
            "slot": "LEFT_RING",
            "label": qsTr("Left Ring")
        }, {
            "slot": "RIGHT_RING",
            "label": qsTr("Right Ring")
        }, {
            "slot": "LEFT_HAND",
            "label": qsTr("Left Hand")
        }, {
            "slot": "RIGHT_HAND",
            "label": qsTr("Right Hand")
        }, {
            "slot": "CHEST",
            "label": qsTr("Chest")
        }, {
            "slot": "GLOVE",
            "label": qsTr("Glove")
        }, {
            "slot": "LEGS",
            "label": qsTr("Legs")
        }, {
            "slot": "FEET",
            "label": qsTr("Feet")
        }]

    function equippedIdItem(slot) {
        for (var i = 0; i < control.equipment.length; i++) {
            if (control.equipment[i].slot === slot) {
                return control.equipment[i].idItem
            }
        }
        return 0
    }

    vTitle: qsTr("Equipment")

    GridLayout {
        anchors.fill: parent
        columns: 2
        rowSpacing: Spaces.spacing8
        columnSpacing: Spaces.spacing8

        Repeater {
            model: root.vSlots

            delegate: ItemSlot {
                required property var modelData

                readonly property int idItem: root.equippedIdItem(modelData.slot)

                Layout.fillWidth: true
                Layout.fillHeight: true
                vLabel: modelData.label
                vItemName: idItem > 0 ? control.itemName(idItem) : ""
            }
        }
    }
}
