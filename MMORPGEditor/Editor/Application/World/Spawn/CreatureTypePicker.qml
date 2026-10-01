import QtQuick
import QtQuick.Controls
import MMORPGUIComponents
import MMORPGEditorControls

Popup {
    id: root

    signal creatureTypeSelected(int type, string name)

    width: 220
    height: 280
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    padding: Spaces.spacing8

    background: PanelFrame {
        vBackgroundColor: Colors.background1
        vBorderColor: Colors.border
        vBorderWidth: Borders.border1
        vRadiusValue: Radiuses.radius8
    }

    CreatureTypePaletteModel {
        id: paletteModel
    }

    ListView {
        anchors.fill: parent
        clip: true
        spacing: Spaces.spacing8
        model: paletteModel

        delegate: ButtonBase {
            required property int type
            required property string name

            width: ListView.view.width
            height: 32
            vBackgroundColor: Colors.background1
            vHoverColor: Colors.background2
            vText: name
            vRadius: Radiuses.radius8

            onClicked: {
                root.creatureTypeSelected(type, name)
                root.close()
            }
        }
    }
}
