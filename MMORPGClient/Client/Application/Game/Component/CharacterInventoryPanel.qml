import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGClientControls

GamePanelBase {
    id: root

    // TODO: Provisional fixed capacity — Backlog "quantos slots o personagem ganha"
    // (ROADMAP.md, Fase P passo 5), possivelmente por Level quando esse sistema existir.
    readonly property int vCapacity: 20

    CharacterInventoryPanelControl {
        id: control
    }

    function itemAt(position) {
        for (var i = 0; i < control.inventory.length; i++) {
            if (control.inventory[i].position === position) {
                return control.inventory[i]
            }
        }
        return null
    }

    vTitle: qsTr("Inventory")
    implicitWidth: 360

    GridLayout {
        anchors.fill: parent
        columns: 5
        rowSpacing: Spaces.spacing8
        columnSpacing: Spaces.spacing8

        Repeater {
            model: root.vCapacity

            delegate: ItemSlot {
                required property int index

                readonly property var entry: root.itemAt(index)

                Layout.fillWidth: true
                Layout.fillHeight: true
                vItemName: entry ? control.itemName(entry.idItem) : ""
                vIconSource: entry ? "image://ClientItemIcon/" + entry.idItem : ""
                vAmount: entry ? entry.amount : 0
            }
        }
    }
}
