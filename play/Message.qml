import QtQuick 2.2

Rectangle {
    id: root
    width: 400
    height: 100
    radius: 6
    color: "black"

    Image {
        source: "images/nuclear.png"
        width: 32; height: 32
        smooth: true; antialiasing: true
        x: 10; y: 10
    }

    //NumberAnimation { id: anim; target: root; property: "height"; duration: 500; to: 100; running: true }
//    Component.onCompleted: anim.start()
}
