import QtQuick
import MMORPGUIComponents

Item {
    id: root

    enum Status {
        Offline,
        Connecting,
        Online
    }

    property int vStatus: ServerStatus.Status.Offline

    implicitWidth: statusLayout.implicitWidth
    implicitHeight: 40

    Row {
        id: statusLayout

        anchors.centerIn: parent
        spacing: Spaces.spacing8

        Item {
            width: 16
            height: 16
            anchors.verticalCenter: parent.verticalCenter

            Rectangle {
                id: pulseCircle

                anchors.fill: parent
                radius: width / 2
                color: indicator.color
                opacity: 0.0

                SequentialAnimation {
                    running: true
                    loops: Animation.Infinite

                    ParallelAnimation {
                        NumberAnimation {
                            target: pulseCircle
                            property: "scale"
                            from: 1.0
                            to: 2.0
                            duration: 1000
                            easing.type: Easing.OutSine
                        }

                        NumberAnimation {
                            target: pulseCircle
                            property: "opacity"
                            from: 0.6
                            to: 0.0
                            duration: 1000
                            easing.type: Easing.OutSine
                        }
                    }

                    PauseAnimation {
                        duration: 200
                    }
                }
            }

            Rectangle {
                id: indicator

                anchors.centerIn: parent
                width: 10
                height: 10
                radius: width / 2

                Behavior on color {
                    ColorAnimation {
                        duration: 250
                    }
                }

                states: [
                    State {
                        name: "offline"
                        when: root.vStatus === ServerStatus.Status.Offline
                        PropertyChanges {
                            target: indicator
                            color: Colors.error
                        }
                    },
                    State {
                        name: "online"
                        when: root.vStatus === ServerStatus.Status.Online
                        PropertyChanges {
                            target: indicator
                            color: Colors.success
                        }
                    },
                    State {
                        name: "connecting"
                        when: root.vStatus === ServerStatus.Status.Connecting
                        PropertyChanges {
                            target: indicator
                            color: Colors.warning
                        }
                    }
                ]
            }
        }

        Text {
            id: statusText

            anchors.verticalCenter: parent.verticalCenter
            font: Fonts.bodyBold
            color: Colors.text

            states: [
                State {
                    name: "offline"
                    when: root.vStatus === ServerStatus.Status.Offline
                    PropertyChanges {
                        target: statusText
                        text: qsTr("OFFLINE")
                    }
                },
                State {
                    name: "online"
                    when: root.vStatus === ServerStatus.Status.Online
                    PropertyChanges {
                        target: statusText
                        text: qsTr("ONLINE")
                    }
                },
                State {
                    name: "connecting"
                    when: root.vStatus === ServerStatus.Status.Connecting
                    PropertyChanges {
                        target: statusText
                        text: qsTr("CONNECTING...")
                    }
                }
            ]
        }
    }
}
