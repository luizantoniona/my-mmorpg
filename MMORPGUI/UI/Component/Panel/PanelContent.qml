import QtQuick

Item {
    id: root

    default property alias contentData: content.data
    readonly property alias contentItem: content

    anchors {
        left: parent.left
        right: parent.right
        top: parent.top
        bottom: parent.bottom
    }

    Item {
        id: content

        anchors.fill: parent
    }
}
