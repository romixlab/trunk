import QtQuick 2.0

Item {
    id: root
    property double intensity: 0.5
    property int pix_shift: height * (1 - intensity)
    width: 128; height: 230

    Image {
        id: lamp_img
        source: "img/lamp/lamp.png"
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
    }
    Item {
        anchors.bottom: parent.bottom
        clip: true
        width: parent.width; height: parent.height - pix_shift
        Image {
            id: glowImg
            anchors.bottom: parent.bottom
            width: root.width; height: root.height
            fillMode: Image.PreserveAspectFit
            source: "img/lamp/light.png"
        }
    }

    function shift(mouseY) {
        var y = mouseY
        var old_intensity = intensity
        if (y >= height - 5)
            intensity = 0.0
        else if (y <= 5)
            intensity = 1.0
        else intensity = 1.0 - y / height
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onMouseYChanged: shift(mouseY)
        onClicked: {
            if (mouse.modifiers & Qt.ShiftModifier)
                intensity = intensity === 0.0 ? 1.0 : 0.0;
        }
    }
}
