import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Button {
    width: 32; height: 24
    checkable: true

    style: ButtonStyle {
        background: Item {
            clip: true
            Image { id: icon; source: "images/led_button.png"
                y: control.checked ?
                       (control.hovered ? -71 : -47) :
                       (control.hovered ? -24 : 0)
            }
        }
    }
}
