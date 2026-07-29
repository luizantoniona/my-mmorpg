import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    property string vWorldName: ""
    property string vWorldSize: ""
    PanelBase {
        vHeaderType: PanelBase.PanelHeaderType.Headerless
        vContentType: PanelBase.PanelContentType.Content

        ColumnLayout {
            anchors.fill: parent
            spacing: Spaces.spacing8

            RowLayout {
                spacing: Spaces.spacing8
                Layout.fillWidth: true

                ColumnLayout {
                    spacing: Spaces.spacing8
                    Layout.fillWidth: true

                    TextTitle {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignVCenter

                        vText: root.vWorldName
                    }

                    TextTitle {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignVCenter

                        vText: root.vWorldSize
                    }
                }
            }
    }
}
