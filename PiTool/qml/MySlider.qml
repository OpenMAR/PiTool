import QtQuick 2.7
import QtQuick.Controls 2.1

Slider {
      id: control
      background: Rectangle {
          x: control.leftPadding
          y: control.topPadding + control.availableHeight / 2 - height / 2
          implicitWidth: 200
          implicitHeight: 2
          width: control.availableWidth
          height: implicitHeight
          radius: 2
          color: white

          Rectangle {
              width: control.visualPosition * parent.width
              height: parent.height
              color: blue
              radius: 2
          }
      }

      handle: Rectangle {
          x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
          y: control.topPadding + control.availableHeight / 2 - height / 2
          implicitWidth: 16
          implicitHeight: 16
          radius: 8
          color: control.pressed ? "#7F7F7F" : white_pure
          border.color: white
      }
  }

