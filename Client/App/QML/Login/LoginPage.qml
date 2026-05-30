import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Managers

Item {
    id: root

    required property AccountManager vAccountManager

    Rectangle {
        id: background
        anchors.fill: parent
        color: '#f0f0f0'
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 0
        spacing: 0

        Rectangle {
            id: leftPanel
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.5
            color: '#2c3e50'
        }

        Rectangle {
            id: rightPanel
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: '#ffffff'
        }
    }
}
