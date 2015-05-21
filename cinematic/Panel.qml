import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.2

Item {
    id: root
    width: 300
    height: 400
    property bool deploy

    ListModel {
        id: messagesModel
        ListElement {
            imgSource: ""
            msgText: "йукйукукуц"
            leftMessage: false
        }
    }

    function message(text, img, isleft) {
        messagesModel.append({imgSource: img, msgText: text, leftMessage: isleft})
        console.log("appending", isleft)
    }

    signal deployMessage(var text, var img, var left)

    Component {
        id: msgDelegate
        Item {
            id: delegate
            width: 300
            height: 68

            Image {
                id: photo
                anchors {
                    top: parent.top
                    topMargin: 10
                }
                width: 48
                height: width
                fillMode: Image.PreserveAspectCrop
                x: leftMessage ? 10 : 65
                source: imgSource

                Button {
                    id: photoButton
                    visible: !root.deploy
                    anchors.centerIn: parent
                    width: 32
                    text: "..."
                    onClicked: fileDialog.open()
                }
            }

            Rectangle {
                anchors {
                    left: photo.right
                    leftMargin: 10
                    top: parent.top
                    bottom: parent.bottom
                }
                width: 140
                radius: 4
                border.color: "lightgray"
                border.width: 1
            }

            TextArea {
                backgroundVisible: false
                verticalScrollBarPolicy: Qt.ScrollBarAlwaysOff
                frameVisible: false
                textColor: "black"
                anchors {
                    left: photo.right
                    leftMargin: 20
                    top: photo.top
                    right: parent.right
                    rightMargin: 10
                    bottom: parent.bottom
                    bottomMargin: 10
                }

                text: msgText
            }
            FileDialog {
                id: fileDialog
                onAccepted: photo.source = fileDialog.fileUrl
            }
        }
    }

    Rectangle {
        id: headerRect
        width: 300
        height: 28
        color: "steelblue"
        radius: 3
        Rectangle {
            width: parent.width
            height: parent.height / 2
            y: parent.height / 2
            color: parent.color
        }
        TextInput {
            font.bold: true
            font.pointSize: 9
            color: "white"
            text: "Человек"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
        }
        TextInput {
            font.bold: true
            font.pointSize: 9
            color: "white"
            text: "Человек 2"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 10
        }
    }
    Rectangle {
        id: messagesRect
        anchors {
            top: headerRect.bottom
            left: headerRect.left
            right: headerRect.right
        }
        height: 400
        color: "white"
        opacity: 0.9
        border.color: "steelblue"
        border.width: 1

        Item {
            clip: true
            anchors.fill: parent
            ListView {
                id: listView
                anchors.fill: parent
                anchors.margins: 10
                model: messagesModel
                delegate: msgDelegate
                spacing: 15
            }
        }
    }
    Rectangle {
        visible: !root.deploy
        anchors {
            left: headerRect.left
            right: headerRect.right
            top: messagesRect.bottom

        }
        height: 84
        color: "steelblue"
        TextArea {
            id: sendTextArea
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
            height: 55
        }
        Button {
            anchors {
                bottom: parent.bottom
                bottomMargin: 3
                left: parent.left
                leftMargin: 5
            }
            text: "Отправить"
            onClicked: {
                messagesModel.append({msgText: sendTextArea.text, leftMessage: true})
            }
        }
        Button {
            anchors {
                bottom: parent.bottom
                bottomMargin: 3
                right: parent.right
                rightMargin: 5
            }
            text: "Отправить"
            onClicked: {
                messagesModel.append({msgText: sendTextArea.text, leftMessage: false})
            }
        }
        Button {
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3
            anchors.horizontalCenter: parent.horizontalCenter
            text: "На видео"
            onClicked: {
                var m = messagesModel.get(messagesModel.count - 1)
                console.log(m.msgText)
                root.deployMessage(m.msgText, "", m.leftMessage)
                messagesModel.remove(messagesModel.count - 1)
            }
        }
    }
}
