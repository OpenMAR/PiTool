import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

/*
Window{
    id:dialog;
    property string displayTitle: ""
    property string displayNote:""

    color:"#00000000"
    flags: Qt.SubWindow | Qt.FramelessWindowHint;
    modality: Qt.WindowModal;
    onClosing: destroy()
    visible: true

    Rectangle{
            id:note
            width: parent.width
            height: parent.height
            anchors.centerIn: parent;
            visible: true
            border.color: blue
            border.width: 1
            //anchors.top:icon.bottom;anchors.topMargin: 2
            //anchors.left: icon.right;anchors.leftMargin: 15
            MyLabel{
                id:title
                color:blue
                anchors.left: parent.left;anchors.leftMargin: 5
                text:displayTitle
            }
            MyLabel{
                id:comment
                anchors.left: parent.left;anchors.leftMargin: 5
                text:displayNote
                anchors.top: title.bottom
            }
        }
}
*/



Rectangle{
    property string displayTitle: ""
    property string displayNote:""
    property int mWidth:10
    property int mHeight:10
    property int mtopMargin:2
    property int mleftMargin:15
    property alias note: note
    property int zvalue: 0
    z: zvalue
    Rectangle{
        id:icon
        anchors.left: parent.left
        anchors.top: parent.top
        width: 14
        height: 14
        border.color: blue
        border.width: 1
        radius:7
        MyLabel{
            font.pixelSize:10
            color:blue
            bold:true
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            text: "!"
        }
        MyMouseArea{
            id:mouseArea
            onEntered: {
                note.visible = true;
                zvalue = 1
            }
            onExited: {
                note.visible = false;
                zvalue = 0
            }
        }
    }
    Rectangle{
        id:note
        visible: false
        width: mWidth
        height: mHeight
        border.color: blue
        border.width: 1
        anchors.top:icon.bottom;anchors.topMargin: mtopMargin
        anchors.left: icon.right;anchors.leftMargin: mleftMargin
        MyLabel{
            id:title
            color:blue
            anchors.left: parent.left;anchors.leftMargin: 5
            text:displayTitle
        }
        MyLabel{
            id:comment
            anchors.left: parent.left;anchors.leftMargin: 5
            text:displayNote
            anchors.top: title.bottom
        }
    }

}

