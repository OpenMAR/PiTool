import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 1.2 as Control12

MyDialog2 {
    property alias text: txt.text;
    property alias btnOK: btnOK.mouseArea;
    property var callback:null;

    clientHeight: 180;
    id:messageDialog

    Rectangle{
        parent:clientArea;
        width: parent.width;height: parent.height;
        color: translucent
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
            anchors.horizontalCenter: parent.horizontalCenter
            mouseArea.onClicked: {
                messageDialog.close();
                if(callback!=null){
                    callback();
                }
            }
        }
        Control12.Action{
            shortcut: "return"
            onTriggered: {
                btnOK.mouseArea.onClicked(Qt.LeftButton);
            }
        }
    }

}
