import QtQuick 2.7
import QtQuick.Controls 2.0

Button {
    property alias mouseArea:mouseArea;
    property alias iconSource:image.source
    property alias imageScale:image.scale
    property alias icon:image
    property alias bgColor:bg.color
    property var image_normal;
    property var image_hover;
    property var image_pressed;
    property var space:10
    width:image.width+space;
    height: image.height+space;
    background:Rectangle{
        id:bg
        opacity: 0
    }
    Image{
        id:image
        source:(mouseArea.pressed&&image_pressed)?image_pressed:((parent.hovered&&image_hover)?image_hover:image_normal)
        anchors.centerIn: parent;
        scale: 1;
    }
    MyMouseArea{
        id:mouseArea
    }
}
