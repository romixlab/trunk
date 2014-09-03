import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Slider {
    orientation: Qt.Horizontal
    height: 200
    width: 50

    style: SliderStyle {
//        groove: Image {
//            fillMode: Image.TileHorizontally

//            source: "images/fader-groove.png"

//        }

        handle: Image {
            source: "images/fader.png"
            rotation: control.orientation === Qt.Vertical ?
                          90 : 0
        }
    }
}
