import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: root
    property alias text: label.text
    property bool glow: false
    width: label.width; height: label.height

    Text {
        id: label
        font { family: "Calibry"; pixelSize: 10; italic: true}
        font.capitalization: Font.AllUppercase
        color: root.glow ? "#f38716" : "#adadad"
    }

    Glow {
        enabled: root.glow
        visible: root.glow
        anchors.fill: label
        radius: 8
        samples: 8
        color: "#f38716"
        source: label
        transparentBorder: true
    }
}
