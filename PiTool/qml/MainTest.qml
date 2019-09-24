import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import pimaxvr.PiService 1.0

MyScrollView{
    property int parent_width: parent.width
    property var mMARGIN1:15
    property var mMARGIN2:40
    Rectangle{//联系我们
        width:parent_width/2;height: image.y+image.height;
        anchors.right: parent.right;
        color:cCOLOR_Translucent
        MyLink{
            id:label1;
            anchors.top: parent.top;
            title: qsTr("官方网站: ");
            url:"http://www.pimaxvr.com";
        }
        MyLink{
            id:label2;
            anchors.top: label1.bottom;anchors.topMargin: mMARGIN1;
            title: qsTr("官方论坛: ");
            url:"http://bbs.pimaxvr.com";
        }
        MyLabel{
            id:label3;
            anchors.top: label2.bottom;anchors.topMargin: mMARGIN1;
            text: qsTr("客服热线: 4000-659-129");
            color:cCOLOR_WHITE2;
            font.pixelSize: 16;
        }
        MyLabel{
            id:label4;
            anchors.top: label3.bottom;anchors.topMargin: mMARGIN1;
            text: qsTr("QQ玩家交流群: 329556377");
            color:cCOLOR_WHITE2;
            font.pixelSize: 16;
        }
        MyLabel{
            id:label5;
            anchors.top: label4.bottom;anchors.topMargin: mMARGIN1;
            text: qsTr("QQ售后服务群: 592157147");
            color:cCOLOR_WHITE2;
            font.pixelSize: 16;
        }
        MyLink{
            id:label6;
            anchors.top: label5.bottom;anchors.topMargin: mMARGIN1;
            title: qsTr("联系邮箱: ");
            urlTitle:"feedback@pimaxvr.com"
            url:"mailto:feedback@pimaxvr.com";
        }
        MyLabel{
            id:label7;
            anchors.top: label6.bottom;anchors.topMargin: mMARGIN1;
            text: qsTr("新浪微博: 小派科技");
            color:cCOLOR_WHITE2;
            font.pixelSize: 16;
        }
        MyLabel{
            id:label8;
            anchors.top: label7.bottom;anchors.topMargin: mMARGIN1;
            text: qsTr("微信公众号: 小派科技");
            color:cCOLOR_WHITE2;
            font.pixelSize: 16;
        }
        Image{
            id:image;
            anchors.top: label8.bottom;anchors.topMargin: mMARGIN1;
            anchors.left: parent.left
            width: 184;height: 184;
            source:"qrc:/resource/weixin.jpg"
        }
    }
    Rectangle{
        id:help;
        width:parent_width/2;
        height: more.y+more.height+10;
        anchors.left: parent.left;
        color:cCOLOR_Translucent
        Component.onCompleted: onlineModel.getHelp();
        MyListView{//帮助
            id:helpListView;
            width: parent.width;
            height: contentHeight+10
            orientation: ListView.Vertical;
            spacing: 15;
            model: onlineModel.help.count;
            delegate: helpDelegate;
        }
        Component{
            id:helpDelegate;
            Rectangle{
                property var helpItem : onlineModel.help.get(index);
                width:help.width;
                height: helpItem.group?(index>0?50:text1.height):text2.height;
                color:cCOLOR_Translucent;
                MyLabel{
                    id:text1;
                    font.pixelSize: 16;
                    text: helpItem.title;
                    visible:helpItem.group
                    width: parent.width;
                    anchors.bottom: parent.bottom;
                    color:"#F1F1F1";
                }
                MyLink{
                    id:text2;
                    urlTitle: helpItem.title;
                    url: helpItem.url;
                    fontSize:16;
                    visible: !helpItem.group;
                    width: parent.width;
                }
            }
        }
        MyLink{
            id:more;
            urlTitle: qsTr("了解更多");
            url:onlineModel.helpMore;
            fontSize:16;
            anchors.top: helpListView.bottom;anchors.topMargin: 40;
            anchors.right: parent.right;anchors.rightMargin: 50;
            showLine:true;
        }
    }
}
