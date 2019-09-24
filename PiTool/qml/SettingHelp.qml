import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import pimaxvr.PiService 1.0
import QtQuick.Dialogs 1.2

Rectangle{//client area
    id:helpDialog
    property int currentIndex: 1;
    property int mMARGIN1: 35;
    width: parent.width;height: parent.height;
    property string tab_txt_color: black;
    Rectangle{//sideLeft
        id:sideLeft;
        width: 145;height: 470//parent.height;
        //border.width: 1
        anchors.top:parent.top;anchors.topMargin:30
        color: white_pure;
        MyTabButton{
            id:tab1
            visible: false
            text: qsTr("诊断")
            checked:currentIndex==0;
            anchors.horizontalCenter: parent.horizontalCenter;
            //anchors.left: parent.left;//anchors.leftMargin: mMARGIN1;
            anchors.top: parent.top;
            mouseArea.onClicked: {
                currentIndex = 0;
            }
            txt.color: checked?blue:tab_txt_color
        }
        MyTabButton{
            id:tab2
            text: qsTr("反馈")
            checked:currentIndex==1;
            //anchors.left: parent.left;
            anchors.top: parent.top;
            anchors.right:parent.right;anchors.rightMargin: 30
            mouseArea.onClicked: {
                currentIndex = 1;
            }
            txt.color: checked?blue:tab_txt_color
            font.family: webFont_bold.name;
        }
        MyTabButton{
            id:tab3
            visible: true;
            text: qsTr("隐私说明")
            checked:currentIndex==2;
            //anchors.left:parent.left;
            anchors.top: tab2.bottom;anchors.topMargin: mMARGIN1;
            anchors.right:parent.right;anchors.rightMargin: 30
            mouseArea.onClicked: {
                currentIndex = 2;
            }
            txt.color: checked?blue:tab_txt_color
            font.family: webFont_bold.name;
        }
    }

    Rectangle{//line
         id:line
         width:1;height:470;
         color: white;
         anchors.left: sideLeft.right;
         anchors.top: sideLeft.top;
     }

    Item {
        id: main
        focus: true;
        width:670;height:470;
        anchors.top: parent.top;anchors.topMargin: 30
        anchors.left:line.right;anchors.leftMargin: 30
        property int currentIndex: helpDialog.currentIndex;
        onCurrentIndexChanged:{
            var pageName=['SettingHelpDiagnose','SettingHelpFeedback','SettingHelpAbout'];
            for(var i=0;i<children.length;i++){
                if(i==currentIndex){
                    children[i].visible=true;
                    children[i].z = 1;
                    children[i].enabled = true;
                }else{
                    children[i].visible=false;
                    children[i].z = 0;
                    children[i].enabled = false;
                }
            }
        }
        Loader {
            id:page1
            width:parent.width;height: parent.height;
            source: "SettingHelpDiagnose.qml";
            visible: false;
        }
        Loader{
            id:page2
            width:parent.width;height: parent.height;
            source: "SettingHelpFeedback.qml";
            visible: true;
        }
        Loader {
            id:page3
            width:parent.width;height: parent.height;
            source: "SettingHelpAbout.qml";
            visible: false;
        }

    }
}
