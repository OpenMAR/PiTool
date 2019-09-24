import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

Window{
    id:dialog;
    color:"#00000000"
    flags: Qt.Dialog | Qt.FramelessWindowHint;
    modality: Qt.WindowModal;
    onClosing: destroy()
    visible: true
    property int clientWidth: 650;
    property int clientHeight: 550;
    property var title:"";
    property int titlebar_height: 30;
    property int mMARGIN1: 35;

    property int windowX //用来存储主窗口x坐标
    property int windowY //存储窗口y坐标
    property int xMouse //存储鼠标x坐标
    property int yMouse //存储鼠标y坐标

    width: mainWindow.width; height: mainWindow.height;
    property alias clientArea: clientArea;
    RectangularGlow{
        anchors.fill: mainArea;
        glowRadius: 5;
        cornerRadius:10;
        spread: 0.4;color:"#7F000000";
    }
    Rectangle{
        id:mainArea;
        width: clientWidth;height: clientHeight;
        anchors.centerIn: parent;
        radius: rRADIUS;
        color: black
        clip: true;
        Rectangle{//titlebar
            id:titleBar
            width: parent.width;height: titlebar_height;
            color: translucent
            MouseArea { //拖动事件
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                onPressed: {
                    xMouse = mouseX
                    yMouse = mouseY
                    windowX = dialog.x
                    windowY = dialog.y
                }
                onPositionChanged: {
                    dialog.x = windowX + (mouseX - xMouse)
                    dialog.y = windowY + (mouseY - yMouse)
                    windowX = dialog.x
                    windowY = dialog.y
                }
            }
            ImageButton {//Close Button
                id: btnClose
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter;
                image_normal: "qrc:/resource/quit-n.png"
                mouseArea.onClicked: {
                    dialog.close();
                }
            }
            MyLabel{
               text:title
               anchors.left: parent.left;anchors.leftMargin: 15
               anchors.verticalCenter: parent.verticalCenter;
               color:white;
            }
        }

        Rectangle{//client area
            id:clientArea;
            width: parent.width;height: parent.height-titleBar.height;
            anchors.top: titleBar.bottom;
        }
    }
}
