import QtQuick 2.7
import QtQuick.Controls 2.0

Button{
    width: textSteamVR.width+imageSteamVR.width+5;
    height: textSteamVR.height;
    property alias  mouseArea: mouseAreaSteamVR
    background:Rectangle{
        color: translucent
    }
    Image{
        id:imageSteamVR;
//        visible: false;
        scale: 0.75;
        source:"qrc:/resource/steam.png";
        anchors.left: parent.left;//anchors.leftMargin: 2;
        anchors.verticalCenter: parent.verticalCenter;
    }
    MyLabel{
        id:textSteamVR
        anchors.left: imageSteamVR.right;anchors.leftMargin: 2;
        anchors.verticalCenter: parent.verticalCenter;
        text:qsTr("SteamVR");
        font.pixelSize: 12;
        font.family: webFont.name;
    }


    Rectangle{
        width:parent.width;
        height:1
        visible:parent.hovered||mouseAreaSteamVR.pressed
        color:white
        anchors.bottom: parent.bottom
    }
    MyMouseArea{
        id:mouseAreaSteamVR
        onClicked: {
            console.log("mouseAreaSteamVR.onClicked");
            if(resourceModel.steamVrIsInstalled()){
                resourceModel.launchSteamVr();
//                Qt.openUrlExternally("steam://rungameid/250820");
            }else{
                //showInstallSteamVr("");
            }
        }
    }
}

