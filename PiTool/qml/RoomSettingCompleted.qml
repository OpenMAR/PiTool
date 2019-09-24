import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

MyLocationDialog {
    id:roomSettingCompletedDialog
    property int mMargin: 40
    property int mLeftMargin:55
    property int mRightMargin:55
    title: qsTr("房间设置向导");
    clientWidth: 750;
    clientHeight: 500;
    property bool connect: piService.connect;
    onConnectChanged: {
        if(!connect){
            roomSettingCompletedDialog.close();
        }
    }
    btnClose.onClicked: {
        piService.stopRoomSettingNoteInHmd();
        roomSettingCompletedDialog.close();
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
                text:qsTr("第4步 完成设置");
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
                anchors.top: roomStatusLine.bottom;anchors.topMargin: 15
                text:qsTr("您已成功完成设置，配置已保存。");
            }
        }
        Image{
            id:roomstep0Image
            source:"qrc:/resource/room_ok.png"
            anchors.top: parent.top;anchors.topMargin: 120
            anchors.left: parent.left;anchors.leftMargin: 300
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
                    //roomSettingCompletedDialog.close();
                    changeRoomSettingSource("RoomSettingGroundHeight.qml");
                }
            }
            TextButton{
                id:roomSettingNextButton
                anchors.bottom: parent.bottom;anchors.bottomMargin: 40
                anchors.right: parent.right
                size:large
                enabled: piService.connect&&piService.headTracked
                type:enabled?primary:common
                text: qsTr("完成设置")
                mouseArea.onClicked:{
                    piService.stopRoomSettingNoteInHmd();
                    roomSettingCompletedDialog.close();
                }
            }

        }
    }
}
