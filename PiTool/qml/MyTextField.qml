import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

TextField {
    property bool bold: false
    font.pixelSize: 14
    font.family: bold?webFont_bold.name:webFont.name;
    selectByMouse:true;
    clip: true;
    style: TextFieldStyle {
        textColor: "#434343"
        placeholderTextColor:white;
        background: Rectangle {
            color:translucent
            radius: rRADIUS;
            border.color: control.focus?blue:white
            border.width: 1
        }
        font:control.font;
    }
}
