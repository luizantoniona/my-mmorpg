import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGClientComponents
import MMORPGClientControls

Item {
    id: root

    signal syncSuccess

    property string vTextError: ""
    property string vTextStatus: ""

    SyncPageControl {
        id: control

        onSyncUpdate: function (message) {
            root.vTextStatus = message
        }
        onSyncSucceeded: function () {
            root.syncSuccess()
        }
        onSyncFailed: function (error) {
            root.vTextError = error
        }
    }

    function startSync() {
        root.vTextError = ""
        root.vTextStatus = qsTr("Connecting...")
        control.sync()
    }

    Component.onCompleted: function () {
        root.startSync()
    }

    Rectangle {
        anchors.fill: parent
        color: Colors.background0
    }

    PanelBase {
        anchors.fill: parent
        anchors.margins: Spaces.spacing8
        vHeaderType: PanelBase.PanelHeaderType.Header
        vHeaderTitle: qsTr("Syncing World Data")

        ColumnLayout {
            anchors.centerIn: parent
            spacing: Spaces.spacing8
            width: Math.min(parent.width - Spaces.spacing8 * 4, 420)

            LoadingProgress {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                visible: root.vTextError === ""
                vProgress: control.progress
                vText: root.vTextStatus
                vTextColor: Colors.info
            }

            TextTitle {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                visible: root.vTextError !== ""
                vText: root.vTextError
                vTextColor: Colors.error
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
            }

            ButtonBase {
                Layout.alignment: Qt.AlignHCenter
                visible: root.vTextError !== ""
                vText: qsTr("Retry")

                onClicked: function () {
                    root.startSync()
                }
            }
        }
    }
}
