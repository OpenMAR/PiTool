import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

Window {
    id:checkingDialog
    flags: Qt.SubWindow | Qt.FramelessWindowHint;
    modality: Qt.ApplicationModal
    color:"#B2000000"
    property bool bFirmware: false;
    property int margin: 20;
    property int status: bFirmware?versionModel.firmwareVersion.status:versionModel.piPlayVersion.status;
    property int upgradeFlag: bFirmware?versionModel.firmwareVersion.upgradeFlag:versionModel.piPlayVersion.upgradeFlag;
    onVisibleChanged: {
        if(visible){
            console.log("versionChecking opened");
            versionModel.checkPiPlayVersion(settingModel.buildVersion,settingModel.piplayBetaTest);
        }
    }
    onUpgradeFlagChanged: {
        if(upgradeFlag>0){
            checkingDialog.close();
            showUpgradeConfirmDialog();
        }
    }

    Rectangle{
        width:700;
        height: 370
        color:"#222222"
        radius: 5;
        anchors.centerIn: parent;
        MyLabel{
           text:qsTr("PiPlay更新")
           anchors.left: parent.left;anchors.leftMargin: margin
           anchors.top: parent.top;anchors.topMargin: margin;
           font.pixelSize: 18;
        }

        ImageButton {//Close Button
            id: btnClose
            anchors.right: parent.right;anchors.rightMargin: margin;
            anchors.top: parent.top;anchors.topMargin: margin;
            image_normal: "qrc:/resource/quit-n.png"
            image_hover: "qrc:/resource/quit-h.png"
            image_pressed: "qrc:/resource/quit-c.png"
            mouseArea.onClicked: {
                checkingDialog.close();
            }
        }
        Rectangle{//checking
            id:layoutChecking
            width: parent.width;height: parent.height;
            color:cCOLOR_Translucent;
            visible: status==0;
            MyLabel{
                id:labelUpgrade;
                text:qsTr("正在检查更新")
                font.pixelSize: 36
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.top:parent.top;anchors.topMargin: 80;
            }
            Image{
                source:"qrc:/resource/version-checking.png";
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.top:labelUpgrade.bottom;anchors.topMargin: 40;
            }
        }
        Rectangle{//checked
            width: parent.width;height: parent.height;
            color:cCOLOR_Translucent;
            visible: status!=0&&upgradeFlag==0;
            MyLabel{
                id:labelChecked;
                text:bFirmware?qsTr("您的固件已经是最新版本！"):qsTr("您的PiPlay已经是最新版本！")
                font.pixelSize: 36
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.bottom: parent.verticalCenter;
            }
            RoundButton{
                btnText: qsTr("关闭")
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.top:labelChecked.bottom;anchors.topMargin: 40;
                mouseArea.onClicked:{
                    checkingDialog.close();
                }
            }
        }
    }
}
