import QtQuick 2.7
import QtQuick.Controls 2.0
Rectangle {
    property alias progressColor:progress.color;
    property real minimumValue: 0;
    property real maximumValue:100;
    property real value: 0
    color:"#222222"
    Rectangle{
        id:progress
        height: parent.height;
        width: parent.width*(parent.value-parent.minimumValue)/(parent.maximumValue-parent.minimumValue)
        color:"#d3d3d3"
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter;
    }
}
