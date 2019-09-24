import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

MyDialog2 {
    id:checkingDialog
    property int leftMargin: 20;
    property int topMargin: 10;
    property int rightMargin: 10;
    property int status: versionModel.piPlayVersion.status;
    property int upgradeFlag: versionModel.piPlayVersion.upgradeFlag;
    property int fontSize: 14;
    clientWidth: 350;
    clientHeight: 180;
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
    title:qsTr("PiTool更新");
    Rectangle{
        parent:clientArea;
        width: parent.width;height: parent.height;
        color:translucent
        Rectangle{//checking
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
        Rectangle{//checked
            width: parent.width;height: parent.height;
            color:translucent;
            visible: status!=0&&upgradeFlag==0;
            MyLabel{
                id:labelChecked;
                text:qsTr("您的PiTool已经是最新版本！")
                font.pixelSize: fontSize
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.bottom: parent.verticalCenter;anchors.bottomMargin: 20;
            }
            TextButton{
                text: qsTr("关闭")
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.bottom: parent.bottom;anchors.bottomMargin: mMargin;
                size:large;
                mouseArea.onClicked:{
                    checkingDialog.close();
                }
            }
        }
    }
}
