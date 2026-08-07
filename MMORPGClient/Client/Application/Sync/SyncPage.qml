import QtQuick
import QtQuick.Layouts
import MMORPGUIComponents
import MMORPGClientComponents
import MMORPGClientControls

Item {
    id: root

    signal success

    property string vTextError: ""
    property string vTextStatus: ""

    SyncPageControl {
        id: control

        onSyncUpdate: function (message) {
            vTextStatus = message
        }
        onSyncSucceeded: function () {
            root.success()
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

        TextTitle {
            visible: root.vTextError.length > 0
            vText: root.vTextError
            vTextColor: Colors.error
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
        }
    }
}
