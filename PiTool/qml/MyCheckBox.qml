import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

CheckBox {
    property var color:checked?blue:black;
    property var fontSize:14;
    property var source_enable:checked?"qrc:/resource/select-h.png":"qrc:/resource/select-n.png";
    property var source_disable:checked?"qrc:/resource/checkbox_checked.png":"qrc:/resource/checkbox_unchecked.png";
    id:checkbox
    text: ""

    style: CheckBoxStyle {
        label: MyLabel{
            id:txt
            text: control.text;
            color:control.color
            font.pixelSize: control.fontSize;
            x:10
            font.family: webFont.name;
        }
        indicator: Rectangle {
            implicitWidth: control.fontSize;
            implicitHeight: control.fontSize;
            color: translucent
            Image{
                anchors.centerIn: parent;
                source:control.enabled?source_enable:source_disable;
            }
        }
    }
}
