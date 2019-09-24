import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import pimaxvr.PiService 1.0
import QtQuick.Dialogs 1.2

Rectangle{//client area
    id:helpDialog
    property int currentIndex: 0;
    property int mMARGIN1: 35;
    width: parent.width;height: parent.height;
    property string tab_txt_color: only_tools?white:black;
    Rectangle{//sideLeft
        id:sideLeft;
        width: 160;height: parent.height;
        color: only_tools?black:white;
        MyTabButton{
            id:tab2
            text: qsTr("SteamVR")
            checked:currentIndex==0;
            anchors.horizontalCenter: parent.horizontalCenter;
            //anchors.left: parent.left;
            anchors.top: parent.top;anchors.topMargin: mMARGIN1;
            mouseArea.onClicked: {
                currentIndex = 0;
            }
            txt.color: checked?blue:tab_txt_color
        }
    }
    Item {
        id: main
        focus: true;
        width:(parent.width-sideLeft.width);height:parent.height;
        anchors.top: parent.top;anchors.left: sideLeft.right;
        property int currentIndex: helpDialog.currentIndex;
        onCurrentIndexChanged:{
            var pageName=['SteamItem'];
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
            source: "SteamItem.qml";
            visible: true;
        }

    }
}
