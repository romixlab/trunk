import QtQuick 2.0

Rectangle {
    id: root
    width: 100
    height: 150
    color: "transparent"
    border { color: "#9b9b9b"; width: 1 }
    radius: 6
    property alias text: label.text

    Rectangle {
        width: root.width; height: 11; radius: 6
        color: "#9b9b9b"
    }

    Rectangle {
        y: 6; width: root.width; height: 5
        color: "#9b9b9b"
    }

    Text {
        id: label
        font { family: "Calibry"; italic: true; pixelSize: 9 }
        color: "#1c1c1c"
        anchors.horizontalCenter: root.horizontalCenter
    }
}
