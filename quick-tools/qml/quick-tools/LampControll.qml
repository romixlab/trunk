import QtQuick 2.0

Item {
    id: lamp
    property double intensity: 0.5
    property int pix_shift: height * (1 - intensity)
    width: 128; height: 230

    //signal intensityChanged()

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
        Image{
            anchors.bottom: parent.bottom
            width: lamp.width; height: lamp.height
            fillMode: Image.PreserveAspectFit
            source: "img/lamp/light.png"
        }
        Behavior on height {
            NumberAnimation { duration: 60; easing.type: Easing.Linear; }
        }
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: {
            if (mouse.modifiers & Qt.ShiftModifier) {
                intensity = intensity === 0.0 ? 1.0 : 0.0;
            } else {
                var y = mouseY
                var old_intensity = intensity
                if (y >= height - 5)
                    intensity = 0.0
                else if (y <= 5)
                    intensity = 1.0
                else intensity = 1.0 - y / height
            }

//            if (intensity != old_intensity)
//                intensityChanged(intensity)
        }
    }
}
