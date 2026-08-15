import QtQuick
import MMORPGUIComponents

Item {
    id: root

    /* Header infos*/
    enum PanelHeaderType {
        Header,
        Headerless
    }
    property int vHeaderType: PanelBase.PanelHeaderType.Headerless
    property string vHeaderTitle: ""

    /*Content infos*/
    enum PanelContentType {
        Content,
        Contentless
    }
    property int vContentType: PanelBase.PanelContentType.Content

    /*Footer infos*/
    enum PanelFooterType {
        Footer,
        Footerless
    }
    property int vFooterType: PanelBase.PanelFooterType.Footerless

    default property alias contentData: panelContent.contentData
    readonly property alias contentItem: panelContent.contentItem

    anchors.fill: parent

    PanelFrame {
        anchors.fill: parent
        vBackgroundColor: Colors.background1
        vBorderColor: Colors.border
        vBorderWidth: Borders.borders1
        vRadiusValue: Spaces.spacing8

        PanelHeader {
            id: header

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
            height: root.vHeaderType !== PanelBase.PanelHeaderType.Headerless ? 40 : 0
            visible: root.vHeaderType !== PanelBase.PanelHeaderType.Headerless
            title: root.vHeaderTitle
        }

        PanelContent {
            id: panelContent

            anchors {
                left: parent.left
                right: parent.right
                top: header.bottom
                bottom: parent.bottom
            }
            anchors.margins: Spaces.spacing8
            visible: root.vContentType !== PanelBase.PanelContentType.Contentless
        }
    }
}
