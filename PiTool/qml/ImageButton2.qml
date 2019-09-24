import QtQuick 2.7
import QtQuick.Controls 2.0

Button {
    property alias mouseArea:mouseArea;
    property alias iconSource:image.source
    property alias imageScale:image.scale
    property alias icon:image
    property alias bgColor:bg.color
    property var image_normal;
    property var image_pressed;
    property var space:10
    property var pressStatus: 0
    width:image.width+space;
    height: image.height+space;
    background:Rectangle{
        id:bg
        opacity: 0
    }
    Image{
        id:image
        source:(pressStatus==0)?image_normal:image_pressed
        anchors.centerIn: parent;
        scale: 1;
    }

    MouseArea{
        id:mouseArea
        anchors.fill: parent;
        hoverEnabled: true;
        cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor
        onClicked: {
            if (pressStatus == 0) {
                pressStatus = 1;
            } else {
                pressStatus = 0;
            }
        }
    }

}
