import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Button {
    width: 30; height: 21

    style: ButtonStyle {
        background: Item {
            clip: true
            Image { id: icon; source: "images/button.png"
                y: control.pressed ? -21 : 0
            }
        }
    }
}
