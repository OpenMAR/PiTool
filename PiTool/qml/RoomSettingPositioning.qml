import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

MyLocationDialog {
    id:roomSettingPositionDialog
    property int mMargin: 40
    property int mLeftMargin:55
    property int mRightMargin:55
    title: qsTr("房间设置向导");
    clientWidth: 750;
    clientHeight: 500;
    property bool connect: piService.connect;
    onConnectChanged: {
        if(!connect){
            roomSettingPositionDialog.close();
        }
    }
    btnClose.onClicked: {
        piService.stopRoomSettingNoteInHmd();
        roomSettingPositionDialog.close();
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
                text:qsTr("第1步 建立定位");
            }
            Rectangle{//line
                id:roomStatusLine;
                width: parent.width;height: 1;
                color: black;
                anchors.top:roomStatusTitle.bottom
            }
            MyLabel{
                id:roomStatusNote
                visible: false
                color: "#949494"
                anchors.top: roomStatusLine.bottom;anchors.topMargin:visible?15:0
                text:qsTr("在有限的行动空间中，\n站立或就坐状态下进行VR体验。");
            }
            MyLabel{
                id:hmdStatusNote
                anchors.top: roomStatusLine.bottom;anchors.topMargin: 15
                text:(piService.hmd_name.indexOf("8K")>0&&!piService.lighthouseEnabled)?qsTr(""):qsTr("打开手柄并把手柄和头盔放置于\n定位基站可见的位置。");
            }
            Rectangle{
                id:hmdStatusRadius;
                width: 10;height: 10;
                color: (piService.connect&&piService.headTracked)?blue:red;
                radius:5
                anchors.top:hmdStatusNote.bottom;anchors.topMargin: 15
            }
            MyLabel{
                id:hmdStatusLabel
                anchors.verticalCenter: hmdStatusRadius.verticalCenter
                anchors.left: hmdStatusRadius.right;anchors.leftMargin: 5
                text:piService.connect?(piService.headTracked?qsTr("头盔已连接"):qsTr("未定位到头盔")):qsTr("头盔未连接");
            }
            Rectangle{
                id:controller0StatusRadius;
                width: 10;height: 10;
                color: (piService.controller1&&piService.controller0Tracked)?blue:red;
                visible: piService.controller1
                radius:5
                anchors.top:hmdStatusRadius.bottom;anchors.topMargin: 10
            }
            MyLabel{
                id:controller0StatusLabel
                anchors.verticalCenter: controller0StatusRadius.verticalCenter
                anchors.left: hmdStatusRadius.right;anchors.leftMargin: 5
                visible: controller0StatusRadius.visible
                text:piService.controller1?(piService.controller0Tracked?qsTr("手柄1已连接"):qsTr("手柄1未定位")):qsTr("手柄1未连接")
            }
            Rectangle{
                id:controller1StatusRadius;
                width: 10;height: 10;
                color: (piService.controller2&&piService.controller1Tracked)?blue:red;
                visible: piService.controller2
                radius:5
                anchors.top:controller0StatusRadius.bottom;anchors.topMargin: 10
            }
            MyLabel{
                id:controller1StatusLabel
                anchors.verticalCenter: controller1StatusRadius.verticalCenter
                anchors.left: hmdStatusRadius.right;anchors.leftMargin: 5
                visible: controller1StatusRadius.visible
                text:piService.controller2?(piService.controller1Tracked?qsTr("手柄2已连接"):qsTr("手柄2未定位")):qsTr("手柄2未连接")
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
                source:"qrc:/resource/room_step1.png"
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
                id:roomSettingPreviousButton
                anchors.bottom: parent.bottom;anchors.bottomMargin: 40
                size:large
                text: qsTr("上一步")
                mouseArea.onClicked:{
                    changeRoomSettingSource("RoomSettingStart.qml");
                }
            }
            TextButton{
                id:roomSettingNextButton
                anchors.bottom: parent.bottom;anchors.bottomMargin: 40
                anchors.right: parent.right
                size:large
                enabled: piService.connect&&piService.headTracked
                type:enabled?primary:common
                text: qsTr("下一步")
                mouseArea.onClicked:{
                    changeRoomSettingSource("RoomSettingCalibration.qml");
                }
            }

        }
    }
}
