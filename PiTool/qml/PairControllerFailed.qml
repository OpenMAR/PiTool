import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

MyLocationDialog {
    id:pairFailedDialog
    property int topMargin: 50;
    property int leftMargin: 30;
    property int status: piService.pairStatus;
    title: qsTr("配对新的控制器");
    clientWidth: 550;
    clientHeight: 350;
    property bool connect: piService.connect;
    onConnectChanged: {
        if(!connect){
            pairFailedDialog.close();
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
            text:if(status == 4){
                     return qsTr("未检测到足够的Dongles");
                 }else{
                     return qsTr("未检测到控制器");
                 }
            anchors.left: controllerImage.right;anchors.leftMargin: leftMargin
            anchors.top: parent.top;anchors.topMargin: topMargin+40;
        }
        MyLabel{
            id: guide;
            text:status == 4?qsTr("尝试重新连接头盔或寻求技术支持。"):qsTr("在配对时，确保控制器电力充足并在头戴式显示器附近。\n请点击下方重试，再次尝试。")
            anchors.left: controllerImage.right;anchors.leftMargin: leftMargin
            anchors.top: foundControllerLable.bottom;anchors.topMargin: 10;
        }
        TextButton{
            id:btnRetry
            visible: status != 4
            text: qsTr("重试");
            size:large;
            round:false
            plain:true
            border.color:white
            type:primary;
            anchors.bottom: parent.bottom;anchors.bottomMargin: 20;
            anchors.right: btnCancel.left;anchors.rightMargin: 20
            mouseArea.onClicked: {
                pairFailedDialog.close();
                showPairControllerDialog();
            }
        }
        TextButton{
            id:btnCancel
            text: status == 4?qsTr("确定"):qsTr("取消");
            size:large;
            round:false
            plain:true
            border.color:white
            type:primary;
            anchors.bottom: parent.bottom;anchors.bottomMargin: 20;
            anchors.right: parent.right;anchors.rightMargin: 20
            mouseArea.onClicked: {
                pairFailedDialog.close();
            }
        }
    }

}
