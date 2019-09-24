import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

MyLocationDialog {
    id:controllerUsingDialog
    property int topMargin: 50;
    property int leftMargin: 30;
    property int status: piService.pairStatus;
    title: qsTr("两个控制器已在使用中");
    clientWidth: 550;
    clientHeight: 350;
    property bool connect: piService.connect;
    onConnectChanged: {
        if(!connect){
            controllerUsingDialog.close();
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
            text:qsTr("要配对额外的控制器，至少其中一个控制器必须被关闭。\n一次只能连接两个控制器。")
            anchors.left: controllerImage.right;anchors.leftMargin: leftMargin
            anchors.top: parent.top;anchors.topMargin: topMargin+40;
        }

        TextButton{
            id:btnOK
            text: qsTr("确定");
            size:large;
            round:false
            plain:true
            border.color:white
            type:primary;
            anchors.bottom: parent.bottom;anchors.bottomMargin: 20;
            anchors.right: parent.right;anchors.rightMargin: 20
            mouseArea.onClicked: {
                controllerUsingDialog.close();
            }
        }
    }

}
