import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2
import pimaxvr.ResourceItem 1.0
import QtGraphicalEffects 1.0

Rectangle{
    id:mainLayout
    width:parent.width;height: parent.height;
    anchors.horizontalCenter: parent.horizontalCenter;
    color:translucent;
    property int mMARGIN : 30
    property int mTopMargin : 60
    property int mLeftMargin : 65//80
    property int mControllerMargin:45
    property int mControllerTopMargin:35
    property int verticalMargin: (height-640)/3;
    property string cCOLOR_DISCONNECT: "#FF0000"
    property string cCOLOR_CONNECT: black;
    property int dialogType: 0;//窗口类型,0:已下载，1：正在下载，2：待升级, 3:我的购买
    property int gGRID_WIDTH: 260;
    property int gGRID_HEIGHT:150;
    property int countPerRow: parseInt(width/gGRID_WIDTH);
    property int resCount: resourceModel.resCount;
    property int dlingCount: resourceModel.dlingCount;
    Timer{
        id:checkControllerState;
        interval: 1000;
        repeat: true;
        triggeredOnStart: true;
        onTriggered: {
            piService.checkControllerState();
        }
    }
    Component.onCompleted: {
        checkControllerState.start();
        console.log("ConnectDialog,Component.onCompleted");
        if(piService.noDfuDriver){
            //showNoDfuDriverDialog();
        }
    }
    Rectangle{
        id:statusRect
        width:360;
        height: 250;
        anchors.top: parent.top;anchors.topMargin: 35;
        anchors.left: parent.left;anchors.leftMargin: (piService.connect&&piService.modeType==0)?mLeftMargin:240;
        //border.width: 1
        ConnectDevice{
            id: connectDevice
            width:240;
            height: 190;
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Rectangle{
        id:rightRegion
        width:360;
        height: 190;
        anchors.top: parent.top;anchors.topMargin: 65;
        anchors.left: statusRect.right
        visible: piService.connect&&piService.modeType==0
        Rectangle{
            id:funcLinks
            width:300
            height: piService.controllerPairSupport?150:95
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            //STEAMVR
            TextButton {
                id: btnSteamvr
                width:300
                height: visible?40:0
                round:true
                plain:true
                border.color:blue
                type:primary
                //txt.bold: true
                text: qsTr("启动SteamVR")
                mouseArea.onClicked: {
                    if(resourceModel.steamVrIsInstalled()){
                        resourceModel.launchSteamVr();
                    }else{
                        showInstallSteamVr("");
                    }
                }
                visible: piService.connect&&piService.modeType==0
            }

            Loader{
                id:spaceCalibrate
            }

            TextButton{
                id:btnRoom;
                visible: piService.connect && piService.modeType==0;
                //anchors.left: calibrationLabel.left
                text:qsTr("房间设置")
                width:300
                height: visible?40:0
                anchors.top:btnSteamvr.bottom;anchors.topMargin: 15
                round:true
                plain:true
                border.color:blue
                type:primary
                mouseArea.onClicked: {
                    //showDialog(spaceCalibrate,"RoomSettingStart.qml");//"SettingSpaceCalibrate.qml");
                    showRoomSettingStartDialog();
                }
            }
            TextButton {
                id: btnRencent
                width:300
                height: visible?40:0
                anchors.top:btnSteamvr.bottom;anchors.topMargin: 15
                round:true
                plain:true
                border.color:blue
                type:primary
                //txt.bold: true
                text: qsTr("方向重置")
                //fontSize:20
                mouseArea.onClicked: {
                    if(piService.resetEyePose()){
                        showAlertPopup2(qsTr("方向重置成功!"));
                    }else{
                        showAlertPopup2(qsTr("方向重置失败!"));
                    }

                }
                visible: false//piService.connect&&piService.modeType==0&&!piService.lighthouseEnabled
            }

            TextButton{
                id:pairBtn;
                width:300
                height: visible?40:0
                anchors.top:btnRoom.bottom;anchors.topMargin: 15
                round:true
                plain:true
                border.color:blue
                type:primary
                text: qsTr("手柄配对")
                mouseArea.onClicked: {
                    piService.setPairIndex(0);
                    showPairControllerDialog();
                }
                visible: piService.connect&&piService.controllerPairSupport&&piService.modeType==0
            }
        }
    }

    Rectangle{
        color:translucent;
        border.color: white;
        border.width: 1;
        radius: rRADIUS;
        width: 720;
        height: 190
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top:statusRect.bottom;anchors.topMargin: 20;

        Rectangle{
            width:parent.width;height: parent.height;
            color: translucent
            visible: true
            Image{
                id:image_controller11
                source: (piService.controller1)?
                            "qrc:/resource/controller_status_ready.png":"qrc:/resource/controller_status_off.png"
                anchors.left: parent.left;anchors.leftMargin: mControllerMargin;
                anchors.top:parent.top;anchors.topMargin:mControllerTopMargin
                width: 91;height: 91;

            }
            MyLabel{
                id:image_controller11State
                anchors.top: image_controller11.bottom;anchors.topMargin: 6
                anchors.horizontalCenter: image_controller11.horizontalCenter;
                font.pixelSize:12
                text:piService.controller1?(piService.controller0Tracked?qsTr("已就绪"):qsTr("未定位")):qsTr("未连接")
            }


            Image{
                id:image_controller12
                source: (piService.controller2)?
                            "qrc:/resource/controller_status_ready.png":"qrc:/resource/controller_status_off.png"
                anchors.left: image_controller11.right;anchors.leftMargin: 2*mControllerMargin;
                anchors.top:parent.top;anchors.topMargin:mControllerTopMargin
                width: 91;height: 91;
            }
            MyLabel{
                id:image_controller12State
                anchors.top: image_controller12.bottom;anchors.topMargin: 6
                anchors.horizontalCenter: image_controller12.horizontalCenter;
                font.pixelSize:12
                text:piService.controller2?(piService.controller1Tracked?qsTr("已就绪"):qsTr("未定位")):qsTr("未连接")
            }

            Image{
                id:image_tracker11;
                source: (piService.tracker1)?
                            "qrc:/resource/base_status_ready.png":"qrc:/resource/base_status_off.png"
                anchors.left: image_controller12.right;anchors.leftMargin: 2*mControllerMargin;
                anchors.top:parent.top;anchors.topMargin:mControllerTopMargin
                width: 91;height: 91;
            }
            MyLabel{
                id:image_tracker11State
                anchors.top: image_tracker11.bottom;anchors.topMargin: 6
                anchors.horizontalCenter: image_tracker11.horizontalCenter;
                font.pixelSize:12
                text:piService.tracker1?(piService.base0Tracked?qsTr("已就绪"):qsTr("未定位")):qsTr("未连接")
            }
            Image{
                id:image_tracker12;
                source: (piService.tracker2)?
                            "qrc:/resource/base_status_ready.png":"qrc:/resource/base_status_off.png"
                anchors.left: image_tracker11.right;anchors.leftMargin: 2*mControllerMargin;
                anchors.top:parent.top;anchors.topMargin:mControllerTopMargin
                width: 91;height: 91;
            }
            MyLabel{
                id:image_tracker12State
                anchors.top: image_tracker12.bottom;anchors.topMargin: 6
                anchors.horizontalCenter: image_tracker12.horizontalCenter;
                font.pixelSize:12
                text:piService.tracker2?(piService.base1Tracked?qsTr("已就绪"):qsTr("未定位")):qsTr("未连接")
            }
        }

    }

}
