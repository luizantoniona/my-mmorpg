import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGClientComponents
import MMORPGClientControls

Item {
    id: root

    signal logoutSuccess

    property string vTextError: ""

    AccountPageControl {
        id: control

        onLogoutSucceeded: function () {
            root.logoutSuccess()
        }
        onLogoutFailed: function (error) {
            root.vTextError = error
        }
    }

    ColumnLayout {
        anchors.fill: parent

        PanelBase {

            ColumnLayout {
                anchors.centerIn: parent
                spacing: Spaces.spacing8

                ButtonBase {
                    Layout.alignment: Qt.AlignHCenter

                    vText: qsTr("Logout")

                    onClicked: function () {
                        control.logout()
                    }
                }

                TextTitle {
                    visible: root.vTextError.length > 0
                    vText: root.vTextError
                    vTextColor: Colors.error
                    horizontalAlignment: Text.AlignHCenter
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }
    }
}
