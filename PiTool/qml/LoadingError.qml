import QtQuick 2.7
import QtQuick.Controls 2.0

Rectangle {
    property alias mouseArea:myLink.mouseArea;
    width: parent.width;
    height: parent.height
    color:cCOLOR_Translucent
    Image{
        id:image;
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.verticalCenter;
        source: "qrc:/resource/none.png";
    }

    MyLink{
        id:myLink
        anchors.top:image.bottom;anchors.topMargin: 20;
        anchors.horizontalCenter: parent.horizontalCenter;
        title: qsTr("网络不好,")
        urlTitle:qsTr("刷新试试");
        showLine:true;
    }
}
