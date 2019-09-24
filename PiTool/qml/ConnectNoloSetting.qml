import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

MyDialog2 {
    id:connectNoloSettingDialog
    property int margin: 20;
    clientWidth: settingModel.mLanguage==0?350:650;
    clientHeight: 200

    Rectangle{
        parent:clientArea;
        width: parent.width;height: parent.height;
        color:translucent
        Rectangle{
            color:translucent
            anchors.horizontalCenter: parent.horizontalCenter
            width: btnChange.x+btnChange.width;
            anchors.top: parent.top;anchors.topMargin: margin;
            MyLabel{
                id:label1
                text:qsTr("定位方案：")
            }
            MyLabel{
                id:displayMode;
                anchors.left: label1.right;anchors.leftMargin: 10;
                text:piService.noloDisplayMode==0?qsTr("180°"):qsTr("360°");
            }
            MyLink{
                id: btnChange
                anchors.left: displayMode.right;anchors.leftMargin: 40;
                urlTitle:piService.noloDisplayMode==0?qsTr("更换360°"):qsTr("更换180°");
                fontSize: 14;
                showLine:true;
                mouseArea.onClicked: {
                    var newMode = (piService.noloDisplayMode==0?1:0);
                    if(piService.setNoloDisplayMode(newMode)){
                        showAlertPopup(newMode==0?qsTr("Nolo定位方案更换为180°成功。"):qsTr("Nolo定位方案更换为360°成功。"));
                    }else{
                        showAlertPopup(newMode==0?qsTr("Nolo定位方案更换为180°失败。"):qsTr("Nolo定位方案更换为360°失败。"));
                    }
                }
            }
            MyLink{
                anchors.top: label1.bottom;anchors.topMargin: margin;
                anchors.horizontalCenter: parent.horizontalCenter;
                urlTitle:qsTr("(360°方案慎用，基站需摆放在2.7米以上)")
                url:qsTr("http://bbs.pimaxvr.com/read.php?tid=1659")
                visible:piService.noloDisplayMode==0;
                fontSize: 14;
                showLine:true;
            }
        }
    }
}
