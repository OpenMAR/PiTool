import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

MyDialog2 {
    id:checkingDialog
    property int mMargin: 30;
    property var firmwareVersion: versionModel.firmwareVersion
    property int status: firmwareVersion.status;
    property int upgradeFlag: firmwareVersion.upgradeFlag;
    property int fontSize: 14
    clientWidth: 350;
    clientHeight: 250;
    btnClose.onClicked: {
        if(firmwareVersion.upgradeFlag==1){
            Qt.quit();
        }else{
            dialog.close();
        }
    }
    onVisibleChanged: {
        if(visible){//&&upgradeFlag==0
            console.log("checking DFU firmware opened");
            if(piService.hmd_name.indexOf("8K")>0){
                versionModel.checkFirmwareVersion(piService.fmBuildVersion,settingModel.betaTest,"p2");
            }else{
                versionModel.checkFirmwareVersion(piService.fmBuildVersion,settingModel.betaTest,piService.product_name);
            }
        }
    }
    title:qsTr("固件更新")

    Rectangle{
        parent:clientArea;
        width: parent.width;height: parent.height;
        color:translucent;

        Rectangle{//checking,statue=0
            id:layoutChecking
            width: parent.width;height: parent.height;
            color:translucent;
            visible: status==0;
            MyLabel{
                id:labelUpgrade;
                text:qsTr("正在检查更新...")
                font.pixelSize: fontSize
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.top:parent.top;anchors.topMargin: 60;
            }
            Image{
                source:"qrc:/resource/version-checking.png";
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.top:labelUpgrade.bottom;anchors.topMargin: 20;
                visible: false
            }
        }
        Rectangle{//checked,not found new version
            width: parent.width;height: parent.height;
            color:translucent;
            visible: status!=0&&upgradeFlag==0;
            MyLabel{
                id:labelChecked;
                text:qsTr("您的固件已经是最新版本！")
                font.pixelSize: fontSize
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.bottom: parent.verticalCenter;anchors.bottomMargin: 20;
            }
            TextButton{
                text: qsTr("关闭")
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.bottom: parent.bottom;anchors.bottomMargin: mMargin;
                size:large
                mouseArea.onClicked:{
                    checkingDialog.close();
                }
            }
        }
        Rectangle{//checked,found new version
            width: parent.width;height: parent.height;
            color:translucent;
            visible: status!=0&&upgradeFlag>0
            MyLabel{
                id:labelNewVersion;
                text:qsTr("发现新固件")+firmwareVersion.version
                font.pixelSize: fontSize
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.top:parent.top;anchors.topMargin: 20;
            }
            MyLabel{
                id:textPrompt1
                text: qsTr("升级前，请先关闭运行中的游戏或影视。")
                wrapMode:Text.WordWrap
                width: parent.width*0.8
                horizontalAlignment:Text.AlignLeft;
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.top:labelNewVersion.bottom;anchors.topMargin: 5;
                color:gray
            }
            MyLabel{
                id:textPrompt2
                text: qsTr("升级过程中，头盔指示灯闪烁，完成后结束闪烁。")
                wrapMode:Text.WordWrap
                anchors.horizontalCenter: parent.horizontalCenter;
                width: parent.width*0.8
                anchors.top:textPrompt1.bottom;anchors.topMargin: 5;
                horizontalAlignment:Text.AlignLeft;
                color:gray
            }

            TextButton{
                id:btnUpgrade;
                text: qsTr("立即更新")
                anchors.bottom: parent.bottom;anchors.bottomMargin: mMargin;
                anchors.left: parent.left;anchors.leftMargin: 20;
                size:large;
                type:primary;
                mouseArea.onClicked: {
                    checkingDialog.close();
                    showVersionUpgradeDFUDialog();
                }
            }
            TextButton{
                id:btnClose2;
                text: firmwareVersion.upgradeFlag==1?qsTr("退出PiTool"):qsTr("以后再说")
                anchors.bottom: parent.bottom;anchors.bottomMargin: mMargin;
                anchors.right: parent.right;anchors.rightMargin: 20;
                size:large;
                type:primary;
                mouseArea.onClicked: {
                    if(firmwareVersion.upgradeFlag==1){
                        Qt.quit();
                    }else{
                        checkingDialog.close();
                    }
                }
            }
        }

    }
}
