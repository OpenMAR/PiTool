import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import pimaxvr.PiService 1.0
import QtQuick.Dialogs 1.2

MyScrollView{
    property int parent_width: parent.width
    property int parent_height:parent.height;
    property int help_width: parent_width/2-10;
    property int mMARGIN1:10
    property int mMARGIN2:20
    property int topMARGIN: 35;
    property int leftMARGIN: 40;
    property int rightMARGIN: 30;
    Rectangle{
        visible: true
        border.width: 1
        width: parent.width;
        height:parent_height//image.y+image.height;
        Rectangle{
            id: description;
            width:parent.width;
            //anchors.left: parent.left;anchors.leftMargin: leftMARGIN;
            color:translucent
            MyLabel{
                id:label1;
                anchors.top: parent.top;//anchors.topMargin:mMARGIN1;
                text: qsTr("官方网站 ");
            }

            MyLabel{
                id:supportLabel;
                anchors.top: label1.bottom;anchors.topMargin:mMARGIN1;
                text: qsTr("官方技术支持");
            }
            MyLabel{
                id:label2;
                anchors.top: supportLabel.bottom;anchors.topMargin: mMARGIN1;
                text: qsTr("官方论坛");
            }
            MyLabel{
                id:label3;
                visible: settingModel.mLanguage==0
                anchors.top: label2.bottom;anchors.topMargin: visible?mMARGIN1:0;
                height: visible?21:0
                text: qsTr("客服热线");
            }
            MyLabel{
                id:labelGuide;
                anchors.top: label3.bottom;anchors.topMargin: mMARGIN1;
                text: qsTr("用户手册");
            }
            MyLabel{
                id:label4;
                anchors.top: labelGuide.bottom;anchors.topMargin: visible?mMARGIN1:0;
                text: qsTr("QQ玩家交流群");
                visible: false//settingModel.mLanguage==0
                height: visible?contentHeight+3:0;
            }
            MyLabel{
                id:label5;
                anchors.top: label4.bottom;anchors.topMargin: visible?mMARGIN1:0;
                text: qsTr("QQ售后服务群");
                visible: false//settingModel.mLanguage==0
                height: visible?contentHeight+3:0;
            }
            MyLabel{
                id:label6;
                anchors.top: label5.bottom;anchors.topMargin: mMARGIN1;
                text: qsTr("联系邮箱");
            }
            MyLabel{
                id:label7;
                anchors.top: label6.bottom;anchors.topMargin: visible?mMARGIN1:0;
                text: qsTr("新浪微博");
                visible: settingModel.mLanguage==0
                height: visible?contentHeight+3:0;
            }
            MyLabel{
                id:label8;
                anchors.top: label7.bottom;anchors.topMargin: visible?mMARGIN1:0;
                text: qsTr("微信公众号");
                visible: settingModel.mLanguage==0
                height: visible?contentHeight+3:0;
            }
            MyLabel{
                id:labelSkype;
                anchors.top: label8.bottom;anchors.topMargin: visible?mMARGIN1:0;
                text: qsTr("Skype");
                visible: settingModel.mLanguage==1
                height: visible?contentHeight+3:0;
            }
            MyLabel{
                id:labelFacebook;
                anchors.top: labelSkype.bottom;anchors.topMargin: visible?mMARGIN1:0;
                text: qsTr("Facebook");
                visible: settingModel.mLanguage==1
                height: visible?contentHeight+3:0;
            }
            MyLabel{
                id:labelTwitter;
                anchors.top: labelFacebook.bottom;anchors.topMargin: visible?mMARGIN1:0;
                text: qsTr("Twitter");
                visible: settingModel.mLanguage==1
                height: visible?contentHeight+3:0;
            }
        }

        Rectangle{//联系我们
            id: contact;
            width:parent.width;
            anchors.left: parent.left;anchors.leftMargin: 130;
            color:translucent
            MyLink{
                id:label11;
                y: label1.y;//anchors.topMargin:mMARGIN1;
                urlTitle: qsTr("点击查看")
                url:settingModel.mLanguage==0 ? qsTr("http://cn.pimaxvr.com") :
                                                    qsTr("http://www.pimaxvr.com");
            }

            MyLink{
                id:supportLabel1;
                y: supportLabel.y;
                urlTitle: qsTr("点击查看")
                url:settingModel.mLanguage==0 ?
                    qsTr("https://work.pimaxvr.com:8080/servicedesk/customer/portal/1"):
                    qsTr("https://pimaxvr.com/pages/feedback");
            }
            MyLink{
                id:label21;
                y: label2.y
                urlTitle: qsTr("点击查看")
                url:qsTr("https://forum.pimaxvr.com");
            }
            MyText{
                id:label31;
                y: label3.y
                visible: settingModel.mLanguage==0
                height: visible?21:0
                text: qsTr("4006-129-665");
            }
            MyLink{
                id:labelGuide1;
                y: labelGuide.y
                urlTitle: qsTr("点击查看")
                url:settingModel.mLanguage==0 ? qsTr("http://pisetup.pimaxvr.com/PiTool_User_Manual_cn.pdf"):
                          qsTr("http://pisetup.pimaxvr.com/PiTool_User_Manual_en.pdf");
            }
            MyText{
                id:label41;
                y: label4.y
                text: qsTr("329556377");
                visible: false//settingModel.mLanguage==0
                height: visible?contentHeight+3:0;
            }
            MyText{
                id:label51;
                y: label5.y
                text: qsTr("592157147");
                visible: false//settingModel.mLanguage==0
                height: visible?contentHeight+3:0;
            }
            MyLink{
                id:label61;
                y: label6.y
                urlTitle:qsTr("support@pimaxvr.com")
                url:qsTr("mailto:support@pimaxvr.com");
            }
            MyText{
                id:label71;
                y: label7.y
                text: qsTr("小派科技");
                visible: settingModel.mLanguage==0
                height: visible?contentHeight+3:0;
            }
            MyText{
                id:label81;
                y: label8.y
                text: qsTr("小派科技");
                visible: settingModel.mLanguage==0
                height: visible?contentHeight+3:0;
            }
            MyText{
                id:labelSkypeNum;
                y: labelSkype.y
                text: qsTr("support@pimaxvr.com");
                visible: settingModel.mLanguage==1
                height: visible?contentHeight+3:0;
            }
            MyText{
                id:labelFacebookNum;
                y: labelFacebook.y
                text: qsTr("fb.me/pimaxvr");
                visible: settingModel.mLanguage==1
                height: visible?contentHeight+3:0;
            }
            MyText{
                id:labelTwitterNum;
                y: labelTwitter.y
                text: qsTr("twitter.com/pimaxofficial");
                visible: settingModel.mLanguage==1
                height: visible?contentHeight+3:0;
            }
            MyImage{
                id:image;
                visible: settingModel.mLanguage==0
                bgWidth: 158
                bgHeight: 158
                anchors.top: labelTwitterNum.bottom;anchors.topMargin: mMARGIN1;
                width: 155;height: 155;
                source: "qrc:/resource/weixin.jpg"
            }
        }

        TextButton{
            id:diagnose;
            visible: true
            text: qsTr("环境诊断");
            size: large
            //url:onlineModel.helpMore;
            anchors.bottom: parent.bottom;//anchors.bottomMargin: 20;
            anchors.left: parent.left;anchors.leftMargin: 330
            mouseArea.onClicked: {
                //showDialog(diagnoseDialog,"Diagnose.qml");
                showDiagnoseWaitDialog();
            }
        }
        TextButton{
            id:more;
            visible: true
            text: qsTr("导出日志");//qsTr("了解更多");
            size: large
            //url:onlineModel.helpMore;
            anchors.bottom: parent.bottom;
            anchors.left: diagnose.right;anchors.leftMargin: 20
            mouseArea.onClicked: {
                fileDialog.open();
            }
        }
        FileDialog {
            id: fileDialog
            selectFolder:false
            selectExisting:false;
            folder: shortcuts.desktop
            title: qsTr("导出日志")
            nameFilters: [qsTr("导出日志文件(*.zip)")]
            onAccepted: {
                console.log("fileDialog,",fileDialog.fileUrl);
                piService.writeDeviceInfo();
                hwObject.writeSystemInfo();
                if(settingModel.exportLog(fileDialog.fileUrl)){
                    showConfirmDialog(qsTr("导出日志成功!"));
                }else{
                    showConfirmDialog(qsTr("导出日志失败!"));
                }
                fileDialog.close();
            }
            onRejected: {
                console.log("Canceled")
            }
            Component.onCompleted: visible = false;
        }


    }

}
