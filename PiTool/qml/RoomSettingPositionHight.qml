import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

MyDialog2 {
    id:roomSettingDialog
    property int mMargin: 30
    title: qsTr("房间设置");
    clientWidth: 450;
    clientHeight: 300;
    property bool connect: piService.connect;
    onConnectChanged: {
        if(!connect){
            roomSettingDialog.close();
        }
    }
    Rectangle{
        parent:clientArea;
        width: parent.width;height: parent.height
        color:translucent;

        MyLabel{
            id: trackedLabel;
            visible: !piService.headTracked
            text:qsTr("头戴式显示器未就绪")
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: parent.top;anchors.topMargin: 20;
            color:red
        }
        MyLabel{
            id: guide;
            text:qsTr("将头盔放在稳定的平面上并朝向您想要面向的默认方位，\n接着测量该平面和地面间的垂直距离，并在下方输入(单位:厘米）")
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: trackedLabel.visible?trackedLabel.bottom:parent.top;anchors.topMargin: trackedLabel.visible?20:mMargin;
        }
        MyTextField{
            id: inputHeight
            focus: true
            //anchors.left: guide.left;anchors.leftMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: guide.bottom;anchors.topMargin: mMargin;
            validator: IntValidator {bottom: 0;}
            inputMethodHints: Qt.ImhFormattedNumbersOnly
        }
        TextButton{
            id:btnOK
            text: qsTr("校准");
            enabled: piService.headTracked
            size:large;
            type:enabled?primary:cancel;
            plain:enabled?true:false
            anchors.top: inputHeight.bottom;anchors.topMargin: mMargin;
            anchors.horizontalCenter: inputHeight.horizontalCenter
            mouseArea.onClicked: {
                if(!piService.headTracked){
                    return;
                }

               piService.spaceCalibration(inputHeight.text/100);
               roomSettingDialog.close();
            }
        }
        Image{
            id:image_hmd
            visible: false
            width:164;height: 125;
            source: {
                if(piService.hmd_name.indexOf("4K")>0){
                    return "qrc:/resource/pi_device_4k.png"
                }else if(piService.hmd_name.indexOf("8K")>0){
                    return "qrc:/resource/pi_device_8k.png"
                }else if(piService.hmd_name.indexOf("B1")>0){
                    return "qrc:/resource/pi_device_b1.png"
                }else{
                    return "qrc:/resource/pi_device_4k.png"
                }
            }
            anchors.top: inputHeight.verticalCenter;anchors.topMargin: -30;
            anchors.right: inputHeight.left;anchors.rightMargin: -30;
        }
    }

}
