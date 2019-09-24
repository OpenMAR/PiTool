import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.4

MyDialog2 {
    property alias text: txt.text;
    property alias txt: txt;
    property alias btnOK: btnOK.mouseArea;
    property alias btnCancel:btnCancel.mouseArea;
    property alias btnOKText: btnOK.text;
    property alias btnCancelText: btnCancel.text;

    id:messageDialog
    clientHeight: 180;
    title: ""
    Rectangle{
        parent:clientArea;
        width: parent.width;height: parent.height;
        color:translucent
        Rectangle{
            width: parent.width;
            height: parent.height/2;
            anchors.top:parent.top;anchors.left: parent.left;
            color: translucent
            MyLabel{
                id:txt
                text:""
                width: parent.width*4/5;
                anchors.centerIn: parent;
                wrapMode: Text.WordWrap;
                horizontalAlignment :Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter;
                lineHeight: 1.05
            }
        }

        TextButton{
            id:btnOK
            text: qsTr("确认");
            size:large;
            type:primary;
            anchors.bottom: parent.bottom;anchors.bottomMargin: mMargin;
            anchors.right: parent.horizontalCenter;anchors.rightMargin: mMargin/2;
        }
        TextButton{
            id:btnCancel
            text: qsTr("取消");
            size:large;
//            type:cancel;
            anchors.bottom: parent.bottom;anchors.bottomMargin: mMargin;
            anchors.left: parent.horizontalCenter;anchors.leftMargin: mMargin/2;
            mouseArea.onClicked: {
                messageDialog.close();
            }
        }
    }

    Action {
        shortcut: "Escape"
        onTriggered: {
            console.log("MyMessageDialog key esc pressed");
            if(btnCancel.visible){
                btnCancel.mouseArea.onClicked(Qt.LeftButton);
            }
        }
    }
    Action{
        shortcut: "Return"
        onTriggered: {
            console.log("MyMessageDialog key_enter pressed");
            btnOK.mouseArea.onClicked(Qt.LeftButton);
        }
    }
}
