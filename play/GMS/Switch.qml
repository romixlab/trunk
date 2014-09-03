import QtQuick 2.0

Item {
    id: root
    width: 22 + llabel.width + rlabel.width + 8; height: 15
    clip: false
    property bool on: false
    property string ltext
    property string rtext

    Label {
        id: llabel
        text: ltext
        glow: !root.on
    }

    Item {
        id: iconItem
        anchors.left: llabel.right; anchors.leftMargin: 4
        width: 22; height: 15
        clip: true
        Image {
            id: icon
            source: "images/switch.png"
            y: root.on ? 0 : -15
        }
    }

    Label {
        id: rlabel
        text: rtext
        glow: root.on
        anchors.left: iconItem.right; anchors.leftMargin: 4
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.on = !root.on
    }
}
