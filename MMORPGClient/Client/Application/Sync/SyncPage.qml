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
            vTextStatus = message
        }
        onSyncSucceeded: function () {
            root.syncSuccess()
        }
        onSyncFailed: function (error) {
            vTextError = error
        }
    }

    Component.onCompleted: {
        control.sync()
    }

    ColumnLayout {
        anchors.fill: parent

        LoadingProgress {
            visible: root.vTextStatus.length > 0
            vProgress: 0.65
            vText: root.vTextStatus
            vTextColor: Colors.accent2
            Layout.alignment: Qt.AlignHCenter
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
