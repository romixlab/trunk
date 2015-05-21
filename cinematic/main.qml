import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.2
import QtMultimedia 5.0

ApplicationWindow {
    title: qsTr("Hello World")
    width: 1240
    height: 800
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: messageDialog.show(qsTr("Open action triggered"));
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }


    Window {
        id: sendWindow
        width: 300
        height: 500
        data: Panel {
            width: sendWindow.width
            height: sendWindow.height
            onDeployMessage: {
                console.log("append")
                deployed.message(text, img, left)

            }

        }
        visible:true
    }


    Video {
        id: video
        anchors.fill: parent
        MouseArea {
            anchors.fill: parent
            onClicked: videoDialog.open()
        }
    }

    FileDialog {
        id: videoDialog
        onAccepted: {
            video.source = videoDialog.fileUrl
            video.play()
        }
    }

    Panel {
        id: deployed
        deploy: true

        anchors {
            right: parent.right
            bottom: parent.bottom
            rightMargin: 30
            bottomMargin: 50
        }

    }



}
