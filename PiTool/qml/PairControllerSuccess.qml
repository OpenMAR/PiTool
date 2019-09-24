import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

MyLocationDialog {
    id:pairSuccessDialog
    property int topMargin: 50;
    property int leftMargin: 30;
    property int status: piService.pairStatus;
    title: qsTr("成功");
    clientWidth: 550;
    clientHeight: 350;
    property bool connect: piService.connect;
    onVisibleChanged: {
        if(visible){
            piService.checkControllerState();
        }
    }

    onConnectChanged: {
        if(!connect){
            pairSuccessDialog.close();
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
            text:qsTr("控制器已配对")
            anchors.left: controllerImage.right;anchors.leftMargin: leftMargin
            anchors.top: parent.top;anchors.topMargin: topMargin+40;
        }
        MyLabel{
            id: guide;
            text:
                if(piService.controller1 && piService.controller2){
                    return qsTr("您的控制器已配对完毕并准备就绪。");
                }else{
                    return qsTr("您的控制器已配对完毕并准备就绪。要\n配对另一个控制器，点击下方的配对另一个控制器。");
                }

            anchors.left: controllerImage.right;anchors.leftMargin: leftMargin
            anchors.top: foundControllerLable.bottom;anchors.topMargin: 10;
        }

        TextButton{
            id:btnAnother
            visible: !piService.controller1 || !piService.controller2
            text: qsTr("配对另一个控制器");
            size:large;
            round:false
            plain:true
            border.color:white
            type:primary;
            anchors.bottom: parent.bottom;anchors.bottomMargin: 20;
            anchors.right: btnOK.left;anchors.rightMargin: 20
            mouseArea.onClicked: {
                pairSuccessDialog.close();
                piService.setPairIndex(1);
                showPairControllerDialog();
            }
        }
        TextButton{
            id:btnOK
            text: qsTr("完成");
            size:large;
            round:false
            plain:true
            border.color:white
            type:primary;
            anchors.bottom: parent.bottom;anchors.bottomMargin: 20;
            anchors.right: parent.right;anchors.rightMargin: 20
            mouseArea.onClicked: {
                pairSuccessDialog.close();
            }
        }
    }

}
