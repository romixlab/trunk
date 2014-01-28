import QtQuick 2.0

Item {
    id: toggle
    width: 48
    height: 75 + icon_img.height
    clip: true
    property bool on: false
    property bool rectled: true
    property alias text: label.text
    property string icon: ""
    property int _icon_sz: icon.length > 0 ? width : 0
    property string _icon_on: _icon_sz > 0 ? icon + "_on.png" : ""
    property string _icon_off: _icon_sz > 0 ? icon + "_off.png" : ""

    Text {
        id: label
        font.bold: true
        color: "white"
        anchors.horizontalCenter: parent.horizontalCenter
        y: 0
    }
    Image {
        id: icon_img
        source: on ? _icon_on : _icon_off;
        anchors.top: label.bottom
        anchors.topMargin: 2
        width: _icon_sz; height: _icon_sz
    }
    Image {
        x:0;
        anchors.top: icon_img.bottom
        anchors.topMargin: 2
        source: on ? "img/toggle-switch/up.png" : "img/toggle-switch/down.png"
    }
    MouseArea {
        anchors.fill: parent
        onClicked: on=!on;
    }
    Rectangle {
        id: led
        width: rectled ? parent.width : 6; height: rectled ? 3 : 6;
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        color: on ? "green" : "red"
        radius: rectled ? 0 : 3
    }
}
