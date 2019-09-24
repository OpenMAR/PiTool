import QtQuick 2.7
import QtQuick.Controls 2.0

Rectangle {
    property alias mouseArea:mouseArea;
    property alias txt:txt;
    property int fontSize: 14;
    property string text: "text";
    property int size: auto;//medium,small,mini
    property int type: primary;//common;//primary,success,cancel
    property bool round: true;//是否圆形按钮,
    property bool plain: true;//false;//没有底色
    property bool fixedColor:false
    property string groundColor:blue
    property bool fixedRect:false
    property int mWidth:20
    property int mHeight:20
    width: {
        if(fixedRect){
            return mWidth;
        }

        switch(size){
            case large:
                return 150;
            case medium:
                return 120;
            default:
                return txt.width+20;
        }
    }
    height: {
        if(fixedRect){
            return mHeight;
        }

        switch(size){
            case large:
                return 30;
            case medium:
                return 24;
            default:
                return txt.height+10;
        }
    }
    color:{
        if(fixedColor){
            return groundColor;
        }

        if(plain){
            return translucent;
        }else if(type==primary){
            return blue;
        }else{
            return white;
        }
    }
    radius: round?height/2:rRADIUS;
    border.color: plain?txt.color:color;
    border.width: 1;

    MyLabel{
        id:txt
        anchors.centerIn: parent;
        color: {
            if(fixedColor){
                return white_pure;
            }

            if(!plain&&type!=common){
                return white_pure;
            }else if(type==primary){
                return blue;
            }else{
                return black;
            }
        }
        text:parent.text;
        font.pixelSize: fontSize;
    }
    MyMouseArea{
        id:mouseArea
        onEntered: {
            plain = false;
        }
        onExited: {
            plain = true;
        }
    }
    readonly property int common: 0;
    readonly property int primary: 1;
    readonly property int cancel: 2;

    readonly property int auto: 0;
    readonly property int large: 1;
    readonly property int medium: 2;
    readonly property int small: 3;
    readonly property int mini: 4;
}
