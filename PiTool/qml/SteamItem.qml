import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import pimaxvr.PiService 1.0
import QtQuick.Dialogs 1.2

MyScrollView{
    property int parent_width: parent.width
    property int parent_height:parent.height;
    property int help_width: parent_width/2-10;
    property int mMARGIN1:10
    property int mMARGIN2:20
    property int topMARGIN: 35;
    property int leftMARGIN: 40;
    property int rightMARGIN: 30;
    Rectangle{
        visible: true
        width: parent.width;
        height:parent.height;
        Rectangle{
            id: contact;
            width:parent.width;
            anchors.left: parent.left;anchors.leftMargin: leftMARGIN;
            color:translucent

            MySteamButton{
                id:btnSteamVR
                anchors.top: parent.top;anchors.topMargin:mMARGIN1;
            }
            MyLink{
                id:steamGuide;
                anchors.top: btnSteamVR.bottom;anchors.topMargin:mMARGIN1;
                fontSize:16
                title: qsTr("SteamVR安装教程: ");
                showLine: true
                url:qsTr("http://www.pimaxvr.com");
            }
        }
    }

}
