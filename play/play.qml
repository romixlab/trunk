import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.2
import QtQuick.Controls.Private 1.0
import "GMS" as GMS
import QtMultimedia 5.0

Image {
    id: root
    source: "GMS/images/panel.jpg"

//    Row {
//        x: 50; y: 50
//        spacing: 20
//        GMS.Button {}
//        GMS.ButtonFX {}
//        GMS.Fader {}
//        GMS.GroupBox { text: "Head"}
//        GMS.Knob {}
//        GMS.Label { text: "Test 123" }
//        GMS.Led {}
//        GMS.LedButton {}
//        GMS.Switch {}
//    }

    QChartGallery {
        anchors.fill: parent

    }

//    GroupBox {
//            anchors.centerIn: parent
//            width: 100
//            height: 200
//            title: "as"
//            style: Style {
//                property Component panel: Rectangle {
//                    color: "gray"
//                    Text {
//                        anchors.horizontalCenter: parent.horizontalCenter
//                        anchors.bottom: parent.top
//                        text: control.title
//                        color: control.enabled ? "red" : "gray"
//                        renderType: Text.NativeRendering
//                        font.italic: !control.enabled
//                        font.weight: Font.Bold
//                        font.pointSize: 18
//                    }
//                }
//            }
//    }


//Message {


//x: 100; y: 100

//}

//    GMS.GroupBox {
//        x: 50; y: 50
//        width: 145
//        text: "LED Controll"

//        Image {
//            id: grooveImg
//            x: 10; y: 32
//            source: "images/groove.png"
//        }

//        Slider {
//            x: 6; y: 35
//            orientation: Qt.Vertical
//            style: SliderStyle {
//                handle: Image {
//                    rotation: 90
//                    source: "GMS/images/fader.png"
//                }
//                groove: Item { implicitWidth: 100; implicitHeight: 8 }
//            }
//        }

//        Row {
//            x: 15; y: 15
//            spacing: 22
//            GMS.Label { text: "R";}
//            GMS.Label { text: "G";}
//            GMS.Label { text: "B";}
//            GMS.Label { text: "LED";}
//        }
//        GMS.Button {
//            id: led_button
//            x: 102; y: 31
//            property bool state: false
//            onClicked: {
//                state = !state
//                //root.ledStateChanged(state)
//            }
//        }
//        Grid {
//            anchors.top: led_button.bottom
//            anchors.horizontalCenter: led_button.horizontalCenter
//            rows: 2; columns: 2
//            GMS.Led { id: led_on; yellow: true; on: led_button.state }
//            GMS.Label { text: "on" }
//            GMS.Led { id: led_off; yellow: true; on: !led_button.state }
//            GMS.Label { text: "off" }
//        }
//    }

}
