import QtQuick 2.7
import QtQuick.Controls 2.0

Rectangle {
    color:black;//"#00000000"
    property alias source: fgImage.source;
    property alias bg_source: bgImage.source;
    property var bgWidth:parent.width;
    property var bgHeight:parent.height;
    Image{
        id:bgImage
        width: bgWidth;
        height: bgHeight;
        anchors.centerIn: parent;
        source:"qrc:/resource/game_default.png"
    }
    Image{
        id:fgImage;
        visible: progress>=1
        width: parent.width;
        height: parent.height;
        anchors.centerIn: parent;
    }
}
