import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2
import pimaxvr.PiService 1.0

Rectangle{//client area
    id:settingDialog
    property int currentIndex: 0;
    property int mMARGIN1: 35;
    width: parent.width;height: parent.height;
    property string tab_txt_color: black;
    Rectangle{//sideLeft
        id:sideLeft;
        width: 145;height: 470;
        anchors.top:parent.top;anchors.topMargin:30
        color: white_pure;
        MyTabButton{
            id:tab1
            text: qsTr("通用")
            checked:currentIndex==0;
            anchors.top: parent.top;
            anchors.right:parent.right;anchors.rightMargin: 30
            mouseArea.onClicked: {
                currentIndex = 0;
            }
            txt.color: checked?blue:tab_txt_color
            font.family: webFont_bold.name;
        }
        MyTabButton{
            id:tab2
            text: qsTr("头盔")
            checked:currentIndex==1;
            anchors.right:parent.right;anchors.rightMargin: 30
            anchors.top: tab1.bottom;anchors.topMargin: mMARGIN1;
            mouseArea.onClicked: {
                currentIndex = 1;
            }
            txt.color: checked?blue:tab_txt_color
            font.family: webFont_bold.name;
        }
        MyTabButton{
            id:tab3
            visible: false;
            text: qsTr("音频")
            checked:currentIndex==2;
            anchors.right:parent.right;anchors.rightMargin: 30
            anchors.top: tab2.bottom;anchors.topMargin: mMARGIN1;
            mouseArea.onClicked: {
                currentIndex = 2;
            }
            txt.color: checked?blue:tab_txt_color
        }
        MyTabButton {
            id:tab4
            text: qsTr("Brainwarp")
            //visible:piService.connect&&piService.modeType==0&&piService.hmd_name.indexOf("8K")>0
            checked: currentIndex==3;
            anchors.right: parent.right;anchors.rightMargin: 30
            anchors.top: tab2.bottom; anchors.topMargin: mMARGIN1;
            mouseArea.onClicked: {
                currentIndex = 3;
            }
            txt.color: checked?blue:tab_txt_color
            font.family: webFont_bold.name;
        }
        Rectangle{
            id:smart;
            visible:piService.connect//&&piService.modeType==0&&piService.hmd_name.indexOf("8K")>0
            width: parent.width; height: 90;
            anchors.bottom: parent.bottom;
            //border.color: "red";

            Rectangle {
                id:foveated
                visible:piService.foveatedSupport
                MyLabel {
                    id:foveatedLeft
                    font.family: webFont.name
                    //font.pixelSize:12
                    color: blue
                    text: qsTr("固定注视渲染")
                }
                MyLabel {
                    id:foveatedRight
                    font.family: webFont.name
                    //font.pixelSize:12
                    anchors.left: foveatedLeft.right; anchors.leftMargin: 5
                    color: (piService.foveatedStatus == 2) ? blue : white
                    text: !piService.foveatedEnalbe?qsTr("已关闭"):
                          ((piService.foveatedStatus == 1)?qsTr("不生效"):
                           qsTr("生效"))
                }
                width: foveatedLeft.width + foveatedRight.width + 5;height: 21
                anchors.top: parent.top;anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter

            }
            Rectangle {
                id:smartNote
                MyLabel {
                    id:smartNoteLeft
                    text: qsTr("智能补帧")
                    color: blue
                    font.family: webFont.name;
                    anchors.left: parent.left
                }
                MyLabel {
                    id:smartNoteRight
                    color: piService.smartSmoothActive?blue:white
                    font.family: webFont.name;
                    anchors.left: smartNoteLeft.right; anchors.leftMargin:5
                    text: !piService.smartSmoothingEnable?qsTr("已关闭")
                          :(!piService.smartSmoothAvalible?qsTr("不可用")
                          :(piService.smartSmoothActive?qsTr("已生效"):qsTr("不生效")))
                }
                width: smartNoteLeft.width + smartNoteRight.width + 5;height: 21
                anchors.top: foveated.bottom;anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Rectangle {
                id:smart_line
                width: 40; height: 1
                anchors.top:smartNote.bottom;anchors.topMargin: 5
                anchors.right: parent.right;anchors.rightMargin: 30
                anchors.horizontalCenter: parent.horizontalCenter
                color: blue
            }
            MyLabel {
                id:refreshNote
                visible: piService.refreshCounts>1
                text: piService.refreshValues[piService.refreshIndex]
                color: blue
                font.family: webFont.name;
                font.pixelSize:12
                anchors.top:smart_line.bottom
                anchors.horizontalCenter: smart_line.horizontalCenter
            }
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
        property int currentIndex: settingDialog.currentIndex;
        onCurrentIndexChanged:{
            var pageName=['SettingGeneral','SettingHmd','SettingAudio', 'SettingBrainwarp'];
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
            source: "SettingGeneral.qml";
            visible: true;
        }
        Loader{
            id:page2
            width:parent.width;height: parent.height;
            source: "SettingHmd.qml";
            visible: false;
        }
        Loader {
            id:page3
            width:parent.width;height: parent.height;
            source: "SettingAudio.qml";
            visible: false;
        }
        Loader {
            id:page4
            width:parent.width;height: parent.height;
            source: "SettingBrainwarp.qml";
            visible: false;
        }

    }
}
