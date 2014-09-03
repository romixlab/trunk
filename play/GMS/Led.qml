import QtQuick 2.0

Item {
    id: root
    width: 14; height: 13
    clip: true
    property bool on: false
    property bool yellow: false

    Image {
        id: icon
        source: "images/led.png"
        y: root.on ? -13 : 0
        x: root.yellow ? 0 : -14
    }
}
