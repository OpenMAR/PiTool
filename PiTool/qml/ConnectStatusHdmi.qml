import QtQuick 2.7
import QtQuick.Controls 2.0

Rectangle{
    id:rectHdmi;
    property int margin: 20;
    width: hdmiState.x+hdmiState.width+margin;
    height: piService.hdmiState?(imageHdmi.height+margin*2):(detailLink.y+detailLink.height+margin);
    border.width: 1;
    border.color: white;
    radius: 5;
    color: translucent
    Image{
        id:imageHdmi;
        anchors.top: parent.top;anchors.topMargin: margin;
        anchors.left: parent.left;anchors.leftMargin: margin;
        source:piService.hdmiState?'qrc:/resource/HDMI-n.png':'qrc:/resource/HDMI-h.png'
    }
    MyLabel{
        id:labelHdmi;
        text:qsTr("HDMI")
        anchors.left: imageHdmi.right;anchors.leftMargin: 10;
        anchors.verticalCenter: imageHdmi.verticalCenter
        color: piService.hdmiState?white:blue;
    }

    MyLabel{
        id:hdmiState;
        text:piService.hdmiState?qsTr("已连接"):qsTr("未连接")
        anchors.left: labelHdmi.right;anchors.leftMargin: 5;
        anchors.verticalCenter: imageHdmi.verticalCenter
        color: piService.hdmiState?white:blue;
    }
    MyLabel{
        id:textHelp
        width: parent.width-margin*2;
        anchors.left: parent.left;anchors.leftMargin: margin;
        anchors.top: imageHdmi.bottom;anchors.topMargin: margin;
        font.pixelSize: 12;
        text:piService.hdmiState?"":hwObject.hdmiPrompt;
        wrapMode: Text.WordWrap;
        lineHeight: 1.5;
        height:piService.hdmiState?0:contentHeight;
        color: piService.hdmiState?white:blue;
        onLinkActivated: {
            Qt.openUrlExternally(link);
            connectHelpDialog.close();
        }
    }
    MyLink{
        id:detailLink;
        visible:!piService.hdmiState
        anchors.top: textHelp.bottom;anchors.topMargin: margin;
        anchors.right: parent.right;anchors.rightMargin: margin;
        urlTitle:qsTr("详细指导");
        fontSize:12;
        showLine:true;
        urlColor:blue;
        mouseArea.onClicked: {
            Qt.openUrlExternally(qsTr("http://bbs.pimaxvr.com/read.php?tid=811&fid=8&page=1"))
            connectHelpDialog.close();
        }
    }
}

