import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents

Item {
    id: root

    property string vTitle: ""
    property bool vClosable: true

    default property alias contentData: content.data

    signal closed

    implicitWidth: 320
    implicitHeight: 360

    PanelFrame {
        anchors.fill: parent
        vBackgroundColor: Colors.background1
        vBorderColor: Colors.border
        vBorderWidth: Borders.border1
        vRadiusValue: Radiuses.radius8

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                Layout.margins: Spaces.spacing8
                spacing: Spaces.spacing8

                TextTitle {
                    Layout.fillWidth: true
                    vText: root.vTitle
                }

                ButtonBase {
                    visible: root.vClosable

                    Layout.preferredWidth: 24
                    Layout.preferredHeight: 24
                    vBackgroundColor: Colors.background1
                    vHoverColor: Colors.background2
                    vRadius: Radiuses.radius8

                    IconBase {
                        anchors.centerIn: parent
                        vSize: 16
                        vSource: Icons.x
                        vColor: Colors.text
                    }

                    onClicked: root.closed()
                }
            }

            Item {
                id: content

                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: Spaces.spacing8
            }
        }
    }
}
