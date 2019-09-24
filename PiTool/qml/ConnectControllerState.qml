import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2

Rectangle{
    property var deviceName: ""
    property bool connect: false;
    property int battery: 0
    property var cCOLOR_DISCONNECT: "#FF0000"
    property var cCOLOR_CONNECT: black;
    property int mMargin1:10;

    height: labelName.height
    width:batterPercent.x+batterPercent.width;
    color: translucent;
    MyLabel{
        id:labelName
        text: deviceName;
    }
    MyLabel{
        id:status
        text:connect?qsTr("已连接"):qsTr("未连接")
        color:connect?cCOLOR_CONNECT:cCOLOR_DISCONNECT
        anchors.verticalCenter: labelName.verticalCenter
        anchors.left: labelName.right;anchors.leftMargin: mMargin1;
    }
    Image{
        id:imageBattery
        source: "qrc:/resource/battery_empty.png";
        anchors.verticalCenter: labelName.verticalCenter;
        anchors.left: status.right;anchors.leftMargin: mMargin1;
        visible: connect
        Rectangle{
            color: black;
            anchors.verticalCenter: parent.verticalCenter;
            anchors.left: parent.left;anchors.leftMargin: 2;
            height:parent.height-4;
            width: (parent.width-6)*battery/100;
        }
    }
    MyLabel{
        id:batterPercent
        text:battery+"%";
        anchors.verticalCenter: labelName.verticalCenter;
        anchors.left: imageBattery.right;anchors.leftMargin: 5;
        visible: false;//connect
    }
}
