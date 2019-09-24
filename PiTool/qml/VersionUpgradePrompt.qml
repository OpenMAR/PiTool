import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

MyDialog2 {
    id:dialog
    property var piPlayVersion: versionModel.piPlayVersion
    title:qsTr("PiTool更新");
    Rectangle{//checking
        parent:clientArea;
        width: parent.width;height: parent.height;
        color:translucent;
        visible: true;
        MyLabel{
            id:labelPiPlay
            text:"PiTool"
            font.pixelSize: 18;
            anchors.top:parent.top;anchors.topMargin: 40;
            anchors.right: parent.horizontalCenter;
        }
        MyLabel{
            id:textVersion
            text: settingModel.version
            font.pixelSize: 32;
            anchors.top:labelPiPlay.top;anchors.topMargin: 10;
            anchors.left: labelPiPlay.right;anchors.leftMargin: 5;
        }
        MyLabel{
            id:textUpgradeContent;
            text:versionModel.release_note(settingModel.version);
            font.pixelSize: 18;
            lineHeight: 1.2
            anchors.top: labelPiPlay.bottom;anchors.topMargin: 30;
            anchors.horizontalCenter: parent.horizontalCenter;
            width: parent.width*3/4;
            height: 200;
            wrapMode: Text.WordWrap;
            clip:true;
        }
    }

}
