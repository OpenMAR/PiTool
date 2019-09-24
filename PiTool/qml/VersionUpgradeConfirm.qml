import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

MyDialog2 {
    id:dialog
    property int mMargin: 30;
    property var piPlayVersion: versionModel.piPlayVersion
    title:qsTr("PiTool更新");
    btnClose.onClicked: {
        if(piPlayVersion.upgradeFlag == 1 || piPlayVersion.upgradeFlag == 2){
            Qt.quit();
        }else{
            dialog.close();
        }
    }

    Rectangle{
        parent:clientArea;
        width: parent.width;height: parent.height;
        color: translucent
        Rectangle{//checking
            width: parent.width;height: parent.height;
            color:translucent;
            visible: true;
            MyLabel{
                id:labelUpgrade;
                text:qsTr("发现PiTool新版本")+piPlayVersion.version
                font.pixelSize: /*settingModel.language==2?*/14
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.top:parent.top;anchors.topMargin: 20;
            }

            MyLabel{
                id:textUpgradeContent;
                text:piPlayVersion.upgradeContent;
                lineHeight: 1.2
                anchors.top: labelUpgrade.bottom;anchors.topMargin: 20;
                anchors.bottom:btnUpgrade.top;anchors.bottomMargin: 20;
                anchors.horizontalCenter: parent.horizontalCenter;
                width: parent.width*3/4;                
                wrapMode: Text.WordWrap;
                color: gray;
                clip:true;
            }
            TextButton{
                id:btnUpgrade;
                text: qsTr("立即更新")
                anchors.bottom: parent.bottom;anchors.bottomMargin: mMargin;
                anchors.left: parent.left;anchors.leftMargin: 20;
                size:large;
                type:primary;
                mouseArea.onClicked: {
                    dialog.close();
                    showVersionDownloadDialog();
                }
            }
            TextButton{
                id:btnClose2;
                visible: piPlayVersion.upgradeFlag != 1 && piPlayVersion.upgradeFlag != 2
                text: qsTr("以后再说")
                anchors.bottom: parent.bottom;anchors.bottomMargin: mMargin;
                anchors.right: parent.right;anchors.rightMargin: 20;
                size:primary;
                mouseArea.onClicked: {
                    //if(piPlayVersion.upgradeFlag==1){
                    dialog.close();
                }
            }
            TextButton{
                id:appClose2;
                visible: piPlayVersion.upgradeFlag == 1 || piPlayVersion.upgradeFlag == 2
                text: qsTr("退出应用")
                anchors.bottom: parent.bottom;anchors.bottomMargin: mMargin;
                anchors.right: parent.right;anchors.rightMargin: 20;
                size:primary;
                mouseArea.onClicked: {
                    //if(piPlayVersion.upgradeFlag==1){
                    Qt.quit();
                }
            }
        }
    }

}
