import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

Window {
    id:modeSwitch
    width: 100;height: 70;
    flags: Qt.SubWindow | Qt.FramelessWindowHint;
    property bool hovered: btn1.hovered || btn2.hovered
    RectangularGlow{
        anchors.fill: rectModeSwitch;
        glowRadius: 5;
        cornerRadius:10;
        spread: 0.4;color:"#7F000000";
    }
    Rectangle{
        id:rectModeSwitch
        width: parent.width;height: parent.height;
        border.width: 1;border.color: gray;
        color:translucent
        Button{
            id:btn1
            width: parent.width-2;height: parent.height/2-1;
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top;anchors.topMargin: 1
            background: Rectangle{
                color: piService.modeType==0?gray:white
            }

            MyLabel{
                text:qsTr("小派玩家模式")
                anchors.centerIn: parent;
            }
            MyMouseArea{
                id:mouseArea1
                onClicked: {
                    if(piService.modeType==0){
                        return;
                    }
                    if(piService.changeModeType(0)){
                        modeSwitch.visible=false;
                        showModeSwitchWaitDialog();
                    }
                }
            }
        }
        Button{
            id:btn2
            width: parent.width-2;height: parent.height/2-1;
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom;anchors.bottomMargin: 1
            background: Rectangle{
                color: piService.modeType!=0?gray:white
            }
            MyLabel{
                text:qsTr("普通观影模式")
                anchors.centerIn: parent;
            }
            MyMouseArea{
                id:mouseArea2;
                onClicked: {
                    if(piService.modeType==1){
                        return;
                    }
                    if(piService.changeModeType(1)){
                        modeSwitch.visible=false;
                        showModeSwitchWaitDialog();
                    }
                }
            }
        }
    }
}
