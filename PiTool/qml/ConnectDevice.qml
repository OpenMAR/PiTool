import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

Rectangle{
    property bool connectState : piService.connect
    property bool spaceCalibratedState:piService.spaceCalibrated
    id:mainLayout
    color:translucent;
    //border.color: white;
    //border.width: 1;
    radius: rRADIUS;
    Image{
        id:imageDevice
        width:240;
        height: 150;
        source: {
            if(piService.connect && piService.hmd_name.indexOf("4K")>0){
                return "qrc:/resource/pi_device_4k.png"
            }else if(piService.connect && piService.hmd_name.indexOf("8K")>0 && piService.leapMotionPlugin){
                return "qrc:/resource/pi_device_8k_leap.png"
            }else if(piService.connect && piService.hmd_name.indexOf("8K")>0){
                return "qrc:/resource/pi_device_8k.png"
            }else if(piService.connect && piService.hmd_name.indexOf("B1")>0){
                return "qrc:/resource/pi_device_b1.png"
            }else if(piService.connect && piService.hmd_name.indexOf("Pro")>0){
                return "qrc:/resource/pi_device_b1.png"
            }
            else if(piService.connect){
                return "qrc:/resource/pi.png"
            }else{
                return "qrc:/resource/pi_device_off.png"
            }
        }
        smooth:true
    }

    onConnectStateChanged: {
        /*if(!piService.connect && !errorTimer.running){
            errorTimer.start();
        }else{
            errorLoader.sourceComponent = null;
            trackerTimer.start();
        }*/
        if(piService.connect){
            if(piService.hmd_name.indexOf("8K")>0){
                versionModel.checkFirmwareVersion(piService.fmBuildVersion,0,"p2");
            }else{
                versionModel.checkFirmwareVersion(piService.fmBuildVersion,0,piService.product_name);
            }
        }
    }
    onSpaceCalibratedStateChanged: {
        if(!spaceCalibratedState && piService.connect){
            showSpaceCalibrationDialog();
        }
    }


    Component{
        id:errorApp;
        MyLabel{
            text:qsTr("应用组件不完整，建议重新安装")
        }
    }
    Component{
        id:errorSp1;
        MyLink{
            //title: qsTr("官方网站: ");
            urlTitle: qsTr("系统缺少SP1补丁")
            url:qsTr("http://www.pimaxvr.com");
        }
    }
    Component{
        id:errorService;
        MyLink{
            title: qsTr("服务未启动  ");
            urlTitle: qsTr("启动服务")
            showLine: true
            mouseArea.onClicked: {
                diagnoseHandler.startService();
                //showDialog(diagnoseDialog,"Diagnose.qml");
            }
        }
    }
    Component{
        id:errorUsb;
        MyLink{
            urlTitle: qsTr("未检测到设备")
            showLine: true
            mouseArea.onClicked: {
                //showDialog(diagnoseDialog,"Diagnose.qml");
                showDiagnoseWaitDialogForConnect();
            }
        }
    }
    Component{
        id:errorVideo;
        MyLink{
            urlTitle: qsTr("未检测到HDMI/DP")
            url:qsTr("http://www.pimaxvr.com");
        }
    }
    Component{
        id:errorConnect;
        MyLink{
            urlTitle:
                if(piService.errorCode == 30200){
                    return qsTr("当前的模式不支持，切回小派模式？");
                }else if (piService.errorCode != 0){
                    return qsTr("故障检测(")+piService.errorCode+qsTr(")");
                }else{
                    return "";
                }

            visible: !(piService.connect&&piService.modeType==0)
            showLine: true
            mouseArea.onClicked: {
                if(piService.errorCode == 30200){
                     showSwithModePrompt(qsTr("是否切换至小派模式?"),0);
                }else{
                    showDiagnoseWaitDialogForConnect();
                }
            }
        }
    }

    Rectangle{
        width: piService.connect&&piService.modeType==0?textConnect.width:textConnect.width+errorLoader.width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top:imageDevice.bottom;anchors.topMargin: 10
        MyLabel{
            id:textConnect
            text:piService.connect&&piService.modeType==0?(piService.headTracked?piService.displayHmdName:qsTr("未定位")):qsTr("未连接: ")
            color:piService.connect&&piService.modeType==0?cCOLOR_CONNECT:cCOLOR_DISCONNECT

            //anchors.left: imageDevice.left;anchors.leftMargin: piService.connect?50:30
        }
        Loader {
            id:errorLoader
            sourceComponent: errorConnect//getComponent();
            anchors.top:textConnect.top
            anchors.left: textConnect.right;
        }
        MyLabel{
            id:leapConnect
            visible: !untrackerNote.visible && piService.connect&&piService.modeType==0
                     &&piService.leapMotionPlugin&&piService.leapMotionInstalled && piService.leapMotionServiceReady;
            height: visible?contentHeight:0
            anchors.top:textConnect.bottom;anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            text:if(piService.leapMotionServiceReady && piService.leapMotionDeviceReady){
                     return qsTr("");
                 }else if(!piService.leapMotionServiceReady){
                     return qsTr("Leap Motion 服务未就绪");
                 }else if(!piService.leapMotionDeviceReady){
                     return qsTr("Leap Motion 设备未就绪");
                 }
            color:piService.leapMotionServiceReady && piService.leapMotionDeviceReady?cCOLOR_CONNECT:"#ffa800"
        }
        MyLink{
            id:leapServiceStart
            visible: !untrackerNote.visible && piService.connect&&piService.modeType==0
                     &&piService.leapMotionPlugin&&piService.leapMotionInstalled && !piService.leapMotionServiceReady;
            showLine: true
            height: visible?contentHeight:0
            anchors.top:textConnect.bottom;anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            urlTitle: qsTr("服务未就绪，启动Leap Motion服务");
            mouseArea.onClicked:{
                diagnoseHandler.startServiceByName("LeapService");
            }

            urlColor:"#ffa800"
        }
        MyLink{
            id:leapInstall
            visible:!untrackerNote.visible && piService.connect&&piService.modeType==0&&piService.leapMotionPlugin&&!piService.leapMotionInstalled;
            showLine: true
            height: visible?contentHeight:0
            anchors.top:textConnect.bottom;anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            urlTitle: qsTr("下载并安装Leap Motion");
            //url:qsTr("http://pisetup.pimaxvr.com/Leap_Motion_Setup.zip");
            mouseArea.onClicked:{
                showCommonDownloadDialog();
            }
            urlColor:"#ffa800"
        }
        MyLink{
            id:spaceSetupStart
            visible: !untrackerNote.visible && !leapInstall.visible && piService.connect&&piService.modeType==0
                     &&!piService.spaceCalibrated;
            showLine: true
            height: visible?contentHeight:0
            anchors.top:textConnect.bottom;anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            urlTitle: qsTr("点击并运行房间设置");
            mouseArea.onClicked:{
                showSpaceCalibrationDialog();
            }

            urlColor:"#ffa800"
        }
        MyLink{
            id:untrackerNote
            visible: piService.connect&&piService.modeType==0&&!piService.headTracked
            height: visible?contentHeight:0
            anchors.top:textConnect.bottom;anchors.topMargin: 10
            anchors.horizontalCenter: textConnect.horizontalCenter
            title: qsTr("将头盔置于定位基站可见范围内");
            urlColor:"#ffa800"
        }
    }

    Timer{
        id:errorTimer;
        interval: 5000;
        repeat: true;
        triggeredOnStart: false;
        onTriggered: {
            diagnoseHandler.softwareDiagnose();
            errorLoader.sourceComponent = getComponent();
            if(piService.connect){
                errorTimer.stop();
                showSpaceCalibrationDialog();
            }
        }
    }

    Rectangle{
        id:rectDevice;
        visible: false;
        anchors.left: parent.horizontalCenter;
        width: parent.width/2;
        height: labelSwicthMode.y+labelSwicthMode.height;
        anchors.verticalCenter: parent.verticalCenter;
        color:translucent
        MyLabel{
            id:labelDevice
            text:qsTr("设备型号")
            visible: piService.connect
        }
        MyText{
            id:textDevice
            text:piService.hmd_name;
            anchors.left: labelDevice.right;
            anchors.leftMargin: 40;
            anchors.top: labelDevice.top;
            visible: piService.connect
        }
        MyLabel{
            id:labelSerial
            text:qsTr("序列号")
            anchors.top: labelDevice.bottom;
            anchors.topMargin: mMARGIN;
            visible: piService.connect
        }
        MyText{
            id:textSerial
            text:piService.serial_number
            anchors.left: textDevice.left;
            anchors.top:labelSerial.top
            visible: piService.connect
        }

        MyLabel{
            id:labelUsb
            text:qsTr("USB")
            anchors.top: labelSerial.bottom;
            anchors.topMargin: mMARGIN;
        }
        MyLabel{
            id:textUsb
            text:piService.usbState?qsTr("已连接"):qsTr("未识别")
            color:piService.usbState?cCOLOR_CONNECT:cCOLOR_DISCONNECT
            anchors.left: textSerial.left;
            anchors.top:labelUsb.top
        }

        MyLabel{
            id:labelHDMI
            text:qsTr("HDMI/DP")
            anchors.top: labelUsb.bottom
            anchors.topMargin: mMARGIN;
        }
        MyLabel{
            id:textHDMI
            text:piService.hdmiState?qsTr("已连接"):qsTr("未连接")
            color:piService.hdmiState?cCOLOR_CONNECT:cCOLOR_DISCONNECT
            anchors.left: textUsb.left;
            anchors.top:labelHDMI.top
        }
    }
    Loader{
        id: guide;
    }

    function showUnconnectText()
    {
        var str = "";
        if(!piService.connect){
            /*if(!errorTimer.running){
                errorTimer.start();
            }*/
            str = qsTr("未连接: ");
        }else{
            errorLoader.sourceComponent = null;
            str = piService.hmd_name;
        }

        return str;
    }

    function getComponent()
    {
        if(!piService.connect){
           if(diagnoseHandler.appComponentMissed){
               return errorApp;
           }else if(diagnoseHandler.sp1_missed){
               return errorSp1;
           }else if(!diagnoseHandler.serviceRunning){
               return errorService;
           }else if(!diagnoseHandler.usbPlugin){
               return errorUsb
           }else if(!diagnoseHandler.videoPlugin){
               return errorVideo;
           }else{
               return null;
           }
        }
    }

}
