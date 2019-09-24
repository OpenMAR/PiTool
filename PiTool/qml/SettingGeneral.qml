import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import pimaxvr.Setting 1.0
import QtQuick.Dialogs 1.2

MyScrollView{
    property int parent_width: parent.width
    property int parent_height: parent.height;
    property var mMARGIN1:40;
    property var mMARGIN2:18;
    property var mMARGIN3:30;
    Rectangle{
        width: parent_width;
        height: parent_height//checkBox4.y+checkBox4.height+20;
        color:translucent
        //version
        MyLabel {
            id:labelVersion;
            anchors.top: parent.top;anchors.topMargin: 5
            text:qsTr("当前版本:")
        }
        MyText{
            id:textVersion;
            text: "V"+settingModel.version
            anchors.left: labelVersion.right
            anchors.leftMargin: 10
            anchors.top: labelVersion.top;
        }
        TextButton{
            id:btnUpgrade
            text: qsTr("检查更新");
            size: large
            anchors.left: textVersion.right;anchors.leftMargin: mMARGIN2;
            //anchors.top: labelVersion.top;
            anchors.verticalCenter: labelVersion.verticalCenter;
            mouseArea.onClicked: {
                showVersionCheckingDialog();
            }
            RedPoint{
                visible: versionModel.piPlayVersion.upgradeFlag>0;
            }
        }


        //language
        MyLabel {
            id: labelLanguage
            anchors.top: btnUpgrade.bottom;anchors.topMargin: mMARGIN3
            text: qsTr("语言切换(重启后生效)")
        }
        MyComboBox {
            id: comboLanguage
            height: 30
            width: 120
            anchors.left: labelLanguage.right;anchors.leftMargin: mMARGIN2;
            anchors.verticalCenter: labelLanguage.verticalCenter;
            currentIndex: settingModel.mLanguage
            model: settingModel.mLanguageList
            isBlack: true;
            onCurrentIndexChanged: {
                if(currentIndex!=settingModel.mLanguage){
                    showMessageDialog(qsTr("重启后生效,是否重启？"),function(){
                        settingModel.mLanguage = currentIndex;
                        settingModel.restart();
                        Qt.quit();
                    },function(){
                        currentIndex = settingModel.mLanguage;
                    });
                }
            }
        }

        MyCheckBox {
            id: checkBox3
            anchors.left: labelLanguage.left;
            anchors.top: labelLanguage.bottom;anchors.topMargin: mMARGIN2
            text: qsTr("开机自启小派工具")
            checked: settingModel.mAutoRun
            onCheckedChanged: {
                console.log("autorun:",checked,settingModel.mAutoRun);
                settingModel.mAutoRun = checked;
            }
        }
        MyCheckBox {
            id: checkBox4
            anchors.left: checkBox3.left;
            anchors.top: checkBox3.bottom;anchors.topMargin: mMARGIN2
            text: qsTr("关闭窗口时，隐藏到任务栏通知区")
            checked: settingModel.mCloseToNotibar
            onCheckedChanged: {
                settingModel.mCloseToNotibar = checked;
            }
        }
        MyCheckBox {
            id: pvrHomeBox
            anchors.left: checkBox4.left;
            anchors.top: checkBox4.bottom;anchors.topMargin: visible?mMARGIN2:0
            text: qsTr("启动Pimax VR Home")
            checked: settingModel.mPvrHomeEnabled
            onCheckedChanged: {
                settingModel.mPvrHomeEnabled = checked;
            }
            visible: piService.modeType==0 && piService.errorCode!=30000
            //height: visible?fontSize:0
        }
        TextButton{
            id:restore
            anchors.bottom: parent.bottom;
            anchors.right: parent.right;anchors.rightMargin: 20
            text: qsTr("恢复默认")
            fontSize: 14;
            size:large
            mouseArea.onClicked: {
                showMessageDialog(qsTr("是否确认将通用设置项恢复默认设置?"),function(){
                    settingModel.restoreToDefault();
                    console.log("autorun restore:",settingModel.mAutoRun);
                    checkBox3.checked = settingModel.mAutoRun;
                    checkBox4.checked = settingModel.mCloseToNotibar;
                    settingModel.mPvrHomeEnabled = true;
                    pvrHomeBox.checked = true;
                },function(){

                });
            }
        }
    }
    function showConfirm1(){
        showAlertPopup(qsTr("路径中仅可存在英文、阿拉伯数字或下划线"));
    }

}
