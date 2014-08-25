import QtQuick 2.0

Item {
    id: root
    width: 48
    height: 51
    property bool on: false

    Image {
        source: on ? "img/toggle-switch/up.png" : "img/toggle-switch/down.png"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: on = !on;
    }
}
