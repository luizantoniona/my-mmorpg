import QtQuick
import QtQuick.Effects
import MMORPGUIComponents

Item {
    id: root

    property string vSource: ""
    property color vColor: Colors.text
    property int vSize: 24

    implicitWidth: root.vSize
    implicitHeight: root.vSize

    Image {
        id: source

        anchors.fill: parent
        visible: false
        source: root.vSource
        sourceSize: Qt.size(root.vSize, root.vSize)
        fillMode: Image.PreserveAspectFit
    }

    MultiEffect {
        anchors.fill: source
        source: source
        brightness: 1.0
        colorization: 1.0
        colorizationColor: root.vColor
    }
}
