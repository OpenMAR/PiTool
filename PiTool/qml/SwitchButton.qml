import QtQuick 2.7
import QtQuick.Controls 2.0
//checked=true:黄底黑边黑字
//checked=false:黑底黑边白字
Rectangle {
    property alias mouseArea:mouseArea;
    property alias text: txt.text;
    property bool checked:false;

    height: 24;width: 100;
    radius: parent.height/2;
    color: checked?blue:white_pure

    MyLabel{
        id:txt
        anchors.centerIn: parent;
        color: checked?white_pure:blue
        bold: checked
    }
    MyMouseArea{
        id:mouseArea
        onEntered: {
           if(!checked){
              border.color = blue;
           }
        }
        onExited: {
            border.color = white_pure;
         }

    }
}
