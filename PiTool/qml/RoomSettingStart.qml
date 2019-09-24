import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

MyLocationDialog {
    id:roomSettingStartDialog
    property int mMargin: 40
    property int mLeftMargin:55
    property int mRightMargin:55
    title: qsTr("房间设置向导");
    clientWidth: 750;
    clientHeight: 500;
    property bool connect: piService.connect;
    onConnectChanged: {
        if(!connect){
            roomSettingStartDialog.close();
        }
    }
    btnClose.onClicked: {
        piService.stopRoomSettingNoteInHmd();
        roomSettingStartDialog.close();
    }

    Rectangle{
        parent:clientArea
        anchors.top:parent.top;anchors.topMargin: mMargin
        Rectangle{
            id:statusRect
            width: 240
            height: 270
            anchors.left: parent.left;anchors.leftMargin:mLeftMargin
            MyLabel{
                id:roomStatusTitle
                bold:true
                font.pixelSize: 18
                color: black
                text:qsTr("房间尚未设置");
            }
            Rectangle{//line
                id:roomStatusLine;
                width: parent.width;height: 1;
                color: black;
                anchors.top:roomStatusTitle.bottom
            }
            MyLabel{
                id:roomStatusNote
                color: "#949494"
                anchors.top: roomStatusLine.bottom;anchors.topMargin: 10
                text:qsTr("请进行房间设置以开始使用头戴显示器");
            }
        }
        Rectangle{
            id:pictureRect
            width:360
            height: 270
            anchors.left: statusRect.right;anchors.leftMargin: mMargin
            border.color: white;
            border.width: 1;
            radius: rRADIUS;
            Image{
                id:roomstep0Image
                source:"qrc:/resource/room_step0.png"
                anchors.centerIn: parent;
            }
        }
        Rectangle{
            id:buttonRect
            width: 640
            height: 150
            anchors.left: parent.left;anchors.leftMargin:mLeftMargin
            anchors.top: statusRect.bottom
            TextButton{
                id:roomSettingButton
                anchors.top: parent.top;anchors.topMargin: 45
                anchors.horizontalCenter: parent.horizontalCenter
                size:large
                text: qsTr("开始设置")
                mouseArea.onClicked: {
                    changeRoomSettingSource("RoomSettingPositioning.qml");
                }
            }
            MyLink{
                id:roomDelaySettingButton;
                urlTitle: qsTr("稍后设置");
                urlColor:"#7d7d7d"
                anchors.bottom: parent.bottom;anchors.bottomMargin: 40
                anchors.horizontalCenter: parent.horizontalCenter
                alwaysShowLine:true;
                mouseArea.onClicked: {
                    piService.stopRoomSettingNoteInHmd();
                    roomSettingStartDialog.close();
                }
            }
        }
    }
}
