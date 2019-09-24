import QtQuick 2.7
import QtQuick.Controls 2.0
//checked=true:黄底黑边黑字
//checked=false:黑底黑边白字
Rectangle {
    property alias mouseArea:mouseArea;
    property alias font: txt.font;
    property alias txt: txt;
    property string text: ""
    property int fontSize: 18;
    property bool showLine: true;
    property bool checked: false;
    height: txt.height;
    width: txt.width+10;

    color: translucent

    Rectangle{
        color:txt.color
        width: 5;
        height: parent.height-10;
        anchors.verticalCenter: parent.verticalCenter;
        anchors.right: parent.right;
        visible: showLine;
    }
    MyLabel{
        id:txt
        text:parent.text;
        anchors.left: parent.left;
        color: checked?blue:black
        font.pixelSize: fontSize;
    }
    MyMouseArea{
        id:mouseArea
    }
}


