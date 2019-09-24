import QtQuick 2.7
import QtQuick.Controls 2.0

Rectangle{
    id:rectUsb;
    property int margin: 20;
    width: usbState.x+usbState.width+margin;
    height: piService.usbState?(imageUsb.height+margin*2):(textHelp.y+textHelp.height+margin);
    border.width: 1;
    border.color: white;
    radius: 5;
    color: translucent
    Image{
        id:imageUsb;
        anchors.top: parent.top;anchors.topMargin: margin;
        anchors.left: parent.left;anchors.leftMargin: margin;
        source:piService.usbState?'qrc:/resource/USB-n.png':'qrc:/resource/USB-h.png'
    }
    MyLabel{
        id:labelUsb;
        text:qsTr("USB")
        anchors.left: imageUsb.right;anchors.leftMargin: 10;
        anchors.verticalCenter: imageUsb.verticalCenter
        color: piService.usbState?white:blue;
    }

    MyLabel{
        id:usbState;
        text:piService.usbState?qsTr("已连接"):qsTr("未连接")
        anchors.left: labelUsb.right;anchors.leftMargin: 5;
        anchors.verticalCenter: imageUsb.verticalCenter
        color: piService.usbState?white:blue;
    }
    MyLabel{
        id:textHelp
        width: parent.width-margin*2;
        anchors.left: parent.left;anchors.leftMargin: margin;
        anchors.top: imageUsb.bottom;anchors.topMargin: margin;
        font.pixelSize: 12;
        text:piService.usbState?"":hwObject.usbPrompt;
        wrapMode: Text.WordWrap;
        lineHeight: 1.5;
        height:piService.usbState?0:contentHeight;
        color: piService.usbState?white:blue;
        onLinkActivated: {
            Qt.openUrlExternally(link);
            connectHelpDialog.close();
        }
    }
}

