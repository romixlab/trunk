import QtQuick 2.0

Item {
    id: root
    width: 30; height: 31
    property bool on: false
    property alias text: label.text

    Image {
        id: fx
        source: "images/button-fx.png"
    }

    Image {
        id: fxa
        source: "images/button-fx-a.png"
        opacity: 0.0
    }

    Text {
        id: label
        anchors.centerIn: parent
        font { pixelSize: 8; family: "Calibry"}
        font.capitalization: Font.AllUppercase
        color: "#3b3b3b"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.on = !root.on
    }

    states: State {
        name: "active"; when: root.on
        PropertyChanges { target: fx; opacity: 0.0 }
        PropertyChanges { target: fxa; opacity: 1.0 }
    }

    transitions: [
        Transition {
            to: "active"; reversible: true
            NumberAnimation {
                properties: "opacity"
                easing.type: Easing.Linear
                duration: 100
            }

        }
    ]
}
