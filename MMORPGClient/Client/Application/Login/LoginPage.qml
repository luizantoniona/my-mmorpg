import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGClientComponents
import MMORPGClientControls

Item {
    id: root

    LoginPageControl {
        id: control
    }

    // TODO: Add background image
    ColumnLayout {
        anchors.fill: parent

        ServerPanel {
            id: serverPanel

            Layout.fillWidth: true
            Layout.preferredHeight: 200
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: Spaces.spacing8

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                // TODO: LOGO / Background / Something
            }

            AccountPanel {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
