import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2

Rectangle{
    property int mMARGIN: 20;
    id:controllerLayout
    border.color: white;
    border.width: 1;
    radius: rRADIUS;
    Rectangle{//title
        id:title;
        width: parent.width;height: 30;
        color: translucent
        MyLabel{
            id:labelController
            text:qsTr("定位设备")
            anchors.left: parent.left;anchors.leftMargin: mMARGIN;
            anchors.verticalCenter: parent.verticalCenter;
        }
        MyLabel{
            id:textController
            text:piService.controllerState?qsTr("已连接"):qsTr("未连接")
            color:piService.controllerState?cCOLOR_CONNECT:cCOLOR_DISCONNECT
            anchors.verticalCenter: parent.verticalCenter;
            anchors.left: labelController.right;anchors.leftMargin: mMARGIN*2;
        }
        MyLink{
            id:textGuide
            anchors.verticalCenter: parent.verticalCenter;
            anchors.right: parent.right;anchors.rightMargin: mMARGIN;
            urlTitle: qsTr("使用指南")
            url:qsTr("http://bbs.pimaxvr.com/read.php?tid=1618")
            urlColor: black
            fontSize: 14;
            showLine: true
            visible:settingModel.mLanguage==0;
        }
    }
    Rectangle{//line
        id:line;
        width: parent.width;height: 1;
        color: white;
        anchors.top: title.bottom;
    }
    Rectangle{
        width: parent.width/2;
        height: parent.height-title.height-line.height;
        anchors.top: line.bottom
        anchors.left: parent.left
        color: translucent
        Image{
            id:image_controller
            source: (piService.controller1||piService.controller2)?
                        (piService.controllerName=="nolo"?"qrc:/resource/controller_nolo.png":"qrc:/resource/controller_general.png"):"qrc:/resource/controller_general.png"
            anchors.verticalCenter: parent.verticalCenter;
            anchors.left: parent.left;anchors.leftMargin: mMARGIN;
            width: 91;height: 91;
        }
        ConnectControllerState{
            id:controller1
            deviceName:qsTr("手柄1");
            connect:piService.controller1
            battery: piService.controller1Battery;
            anchors.left: image_controller.right;anchors.leftMargin: mMARGIN;
            anchors.verticalCenter: parent.verticalCenter;
        }
        ConnectControllerState{
            deviceName:qsTr("手柄2");
            connect:piService.controller2
            battery: piService.controller2Battery;
            anchors.top: controller1.bottom;anchors.topMargin: mMARGIN;
            anchors.left: controller1.left
        }
        MyLink{
            anchors.bottom: controller1.top;anchors.bottomMargin: mMARGIN;
            anchors.left: controller1.left
            urlTitle: qsTr("手柄配对")
            urlColor: black
            fontSize: 14;
            showLine: true
            visible: false;
            mouseArea.onClicked: {                
                showConfirmDialog(qsTr("功能未实现"));
                //showDialog(noloSetting,"ConnectNoloSetting.qml");
            }
        }
    }
    Rectangle{
        width: parent.width/2;
        height: parent.height-title.height-line.height;
        anchors.top: line.bottom;
        anchors.right: parent.right;
        color: translucent
        Image{
            id:image_tracker;
            source: (piService.controller1||piService.controller2)?
                        (piService.controllerName=="nolo"?"qrc:/resource/tracker_nolo.png":"qrc:/resource/tracker_general.png"):"qrc:/resource/tracker_general.png"
            anchors.verticalCenter: parent.verticalCenter;
            anchors.left: parent.left;anchors.leftMargin: mMARGIN;
            width: 91;height: 91;
        }

        ConnectControllerState{
            id:tracker1
            deviceName:qsTr("基站1");
            connect:piService.tracker1
            battery: piService.tracker1Battery;
            anchors.verticalCenter: parent.verticalCenter;
            anchors.left: image_tracker.right;anchors.leftMargin: mMARGIN;
        }
        ConnectControllerState{
            deviceName:qsTr("基站2");
            connect:piService.tracker2
            battery: piService.tracker2Battery;
            anchors.top: tracker1.bottom;anchors.topMargin: mMARGIN;
            anchors.left: image_tracker.right;anchors.leftMargin: mMARGIN;
            visible: piService.tracker2;
        }
        MyLink{
            id:controllerQuestion
            anchors.bottom: tracker1.top;anchors.bottomMargin: mMARGIN;
            anchors.left: tracker1.left
            urlTitle: qsTr("切换方案")
            urlColor: black
            fontSize: 14;
            showLine: true
            visible: piService.controllerState
            mouseArea.onClicked: {
                createDialog(noloSetting);
            }
        }
    }
    property Component noloSetting:ConnectNoloSetting{
    }
}
