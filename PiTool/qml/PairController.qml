import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

MyLocationDialog {
    id:pairDialog
    property int topMargin: 50;
    property int leftMargin: 30;
    property int status: piService.pairStatus;
    title: qsTr("配对新的控制器");
    clientWidth: 550;
    clientHeight: 350;
    property bool connect: piService.connect;
    btnClose.onClicked: {
        piService.stopPairController();
        waitTimer.stop();
        pairDialog.close();
    }

    onConnectChanged: {
        if(!connect){
            piService.stopPairController();
            waitTimer.stop();
            pairDialog.close();
        }
    }

    onVisibleChanged: {
        if(visible){
            waitTimer.start();
            piService.startPairController();
        }
    }

    Timer{
        id:waitTimer;
        interval: 500;
        repeat: true;
        triggeredOnStart: true;
        onTriggered: {
            if(status==1 || status == 2){
                waitTimer.stop();
                pairDialog.close();
                showPairControllerSuccessDialog();
            }else if(status != 0){
                waitTimer.stop();
                pairDialog.close();
                showPairControllerFailedDialog();
            }
        }
    }

    Rectangle{
        parent:clientArea;
        width: parent.width;height: parent.height
        color:translucent;
        Image{
            id:controllerImage
            source: "qrc:/resource/pairing_controller.png"
            anchors.left: parent.left;anchors.leftMargin: leftMargin;
            anchors.top:parent.top;anchors.topMargin: topMargin
            width: 120;height: 220;
        }
        MyLabel{
            id: foundControllerLable;
            text:qsTr("寻找控制器中...")
            anchors.left: controllerImage.right;anchors.leftMargin: leftMargin
            anchors.top: parent.top;anchors.topMargin: topMargin+40;
        }
        Image{
            id:buttonImage
            source: "qrc:/resource/pairing_buttons.png"
            anchors.left: controllerImage.right;anchors.leftMargin: leftMargin;
            anchors.top:foundControllerLable.bottom;anchors.topMargin: 10
            //width: 120;height: 220;
        }
        MyLabel{
            id: guide;
            text:qsTr("同时按住系统键和菜单键，直到控制器指示灯呈蓝色闪烁\n表明已经进入配对模式。")
            anchors.left: controllerImage.right;anchors.leftMargin: leftMargin
            anchors.top: buttonImage.bottom;anchors.topMargin: 10;
        }

        TextButton{
            id:btnOK
            text: qsTr("取消");
            size:large;
            round:false
            plain:true
            border.color:white
            type:primary;
            anchors.bottom: parent.bottom;anchors.bottomMargin: 20;
            anchors.right: parent.right;anchors.rightMargin: 20
            mouseArea.onClicked: {
                piService.stopPairController();
                waitTimer.stop();
                pairDialog.close();
            }
        }
    }

}
