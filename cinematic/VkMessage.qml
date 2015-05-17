import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Dialogs 1.2

Item {
    width: 250
    height: 110

    Rectangle {
        anchors.fill: parent
        radius: 5
        color: "black"
        opacity: 0.8
    }

    Text {
        id: newMessageText
        text: "Новое сообщение"
        font.bold: true
        font.pointSize: 10
        color: "white"
        anchors {
            top: parent.top
            topMargin: 10
            left: parent.left
            leftMargin: 10
        }
    }

    Image {
        id: photo
        anchors {
            top: newMessageText.bottom
            topMargin: 10
            left: parent.left
            leftMargin: 10
        }
        width: 64
        height: width
        fillMode: Image.PreserveAspectCrop

        Button {
            id: photoButton
            anchors.centerIn: parent
            width: 32
            text: "..."
            onClicked: fileDialog.open()
        }
    }

    TextInput {
        id: senderText
        font.bold: true
        font.pointSize: 9
        color: "steelblue"
        text: "Кто-то"
        anchors {
            left: photo.right
            leftMargin: 10
            top: newMessageText.bottom
            topMargin: 10
        }
    }

    TextInput {
        id: msgText1
        color: "#ffffff"
        font.pointSize: 9
        text: "qeqweqw"
        anchors {
            left: senderText.right
            leftMargin: 5
            top: senderText.top
        }
    }

    TextArea {
        id: msgText2
        textColor: "white"
        backgroundVisible: false
        font.pointSize: 9
        anchors {
            top: senderText.bottom
            topMargin: 1
            left: senderText.left
            right: parent.right
            rightMargin: 10
            bottom: parent.bottom
            bottomMargin: 10
        }
        frameVisible: false
    }

    FileDialog {
        id: fileDialog
        onAccepted: photo.source = fileDialog.fileUrl
    }

    //state: "deploy"
    states: [
        State {
            name: "deploy"
            PropertyChanges { target: photoButton; visible: false }
            PropertyChanges { target: msgText2; verticalScrollBarPolicy: Qt.ScrollBarAlwaysOff }
        }
    ]
}

