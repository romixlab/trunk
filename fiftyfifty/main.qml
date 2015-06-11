import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Material 0.1

ApplicationWindow {
    id: root
    title: qsTr("50/50")
    width: 640
    height: 480
    visible: true
    flags: Qt.FramelessWindowHint


    theme {
            primaryColor: "#2196F3"
            primaryDarkColor: "#1976D2"
            accentColor: "#1976D2"
            tabHighlightColor: "white"
    }

   initialPage: Page {
       id: page
       title: "Fifty Fifty"
       actions: [
           Action {
               name: "Start"
               iconName: "action/alarm_on"
               onTriggered: {
                   progressCircle.indeterminate = false
                   timer.sec = 0
                   timer.running = true
               }
           },
           Action {
               name: "Stop"
               iconName: "av/stop"
               onTriggered: {
                   progressCircle.indeterminate = true
                   timer.sec = 3000
                   timer.running = false
               }
           },
           Action {
               name: "Close"
               iconName: "navigation/close"
               onTriggered: root.showMinimized()
           }
       ]
   }

   function twoZero(num) {
       return num > 9 ? num.toString() : "0" + num
   }

   ProgressCircle {
       id: progressCircle
       anchors.centerIn: parent
       width: Units.dp(300)
       height: Units.dp(300)
       indeterminate: true
       value: timer.sec / 3000

       Label {
           font.family: "Roboto"
           font.weight: Font.Light
           text: {
               var t = twoZero(Math.floor(timer.sec / 60))
               t += ":"
               t += twoZero(Math.floor(timer.sec % 60))
               return t
           }
           font.pixelSize: Units.dp(96)
           anchors.centerIn: parent
       }
   }

   Timer {
       id: timer
       property int sec: 3000
       interval: 1000
       running: false
       repeat: true
       onTriggered: {
           sec++
           if (sec == 3000) {
               timer.running = false
               progressCircle.indeterminate = true
               root.showNormal()
           }
       }
   }

   MouseArea {
       anchors {
           top: parent.top
           left: parent.left
           right: parent.right
           rightMargin: page.actions.length * 50
       }
       height: 50
       propagateComposedEvents: true
       property variant clickPos: "1,1"
       onPressed: clickPos = Qt.point(mouseX, mouseY)
       onPositionChanged: {
           var delta = Qt.point(mouseX - clickPos.x, mouseY - clickPos.y)
           root.x += delta.x
           root.y += delta.y
       }
   }

   Connections {
       target: singleinstance
       onNewInstance: root.showNormal()
   }
}
