import QtQuick 2.7
import QtQuick.Controls 2.0

Rectangle {
    property alias mouseArea:mouseArea;
    property string title:"";
    property string urlTitle:"";
    property string url:"";
    property int fontSize:14;
    property bool showLine:false;
    property bool alwaysShowLine:false;
    property string urlColor: black;
    width: text1.width+text2.width
    height: text1.height
    color:translucent
    MyLabel{
        id:text1
        anchors.left: parent.left
        text:title
        color:urlColor
        font.pixelSize: fontSize;
        font.family: webFont.name;
        anchors.verticalCenter: parent.verticalCenter
    }
    Button{
        id:btnLink
        anchors.left: text1.right
        width: text2.width
        height: text1.height;
        visible: url!=""||urlTitle!=""
        background:Rectangle{
            color: translucent
        }
        MyText{
            id:text2
            anchors.centerIn: parent;
            text:urlTitle==""?url:urlTitle;
            color:(parent.hovered||mouseArea.pressed)?blue:urlColor
            font.pixelSize: fontSize;
            font.family: webFont.name;
            elide:Text.ElideRight;
        }
        Rectangle{
            width:text2.width;
            height:1
            visible:alwaysShowLine || (showLine&&(parent.hovered||mouseArea.pressed))
            color:(parent.hovered||mouseArea.pressed)?blue:urlColor
            anchors.bottom: parent.bottom
        }
        MyMouseArea{
            id:mouseArea
            onClicked: {
                if(url!=""){
                    Qt.openUrlExternally(url);
                }
            }
        }
    }
}

