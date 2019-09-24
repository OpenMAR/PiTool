import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

Window {
    property int mMargin: 30;
    property int leftMargin: 20;
    property int topMargin: 10;
    property int rightMargin: 10;
    property int fontSize: 14;
    property int clientWidth: 350;
    property int clientHeight: 260;
    property var title:""
    property alias clientArea: clientArea;
    property bool showBtnClose: true;
    property alias btnClose:btnClose.mouseArea;

    id:dialog
    flags: Qt.SubWindow | Qt.FramelessWindowHint;
    modality: Qt.ApplicationModal
    color:translucent
    RectangularGlow{
        anchors.fill: main;
        glowRadius: 5;
        cornerRadius:10;
        spread: 0.4;color:"#7F000000";
    }
    Rectangle{
        id:main;
        width:clientWidth;height: clientHeight
        radius: 5;
        anchors.centerIn: parent;
        MyLabel{
           text:title
           anchors.left: parent.left;anchors.leftMargin: leftMargin
           anchors.top: parent.top;anchors.topMargin: topMargin;
           font.pixelSize: fontSize;
        }

        ImageButton {//Close Button
            id: btnClose
            anchors.right: parent.right;anchors.rightMargin: rightMargin;
            anchors.top: parent.top;anchors.topMargin: topMargin;
            image_normal: "qrc:/resource/quit-n.png"
            visible: showBtnClose;
            mouseArea.onClicked: {
                dialog.close();
            }
        }
        Rectangle{
            id:clientArea;
            width: parent.width; height:parent.height-mMargin;
            anchors.top: parent.top;anchors.topMargin: mMargin;
            color:translucent
        }
    }
}
