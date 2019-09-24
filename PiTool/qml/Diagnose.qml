import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import pimaxvr.PiService 1.0
import QtQuick.Dialogs 1.2

Window {
    property int mMargin: 30;
    property int leftMargin: 20;
    property int topMargin: 10;
    property int rightMargin: 10;
    property int fontSize: 14;
    property int clientWidth: 700;
    property int clientHeight: 500;
    property var title:qsTr("连接诊断")
    property alias clientArea: clientArea;
    property bool showBtnClose: true;
    property alias btnClose:btnClose.mouseArea;
    property int diagnoseLabelWidth:210
    property int diagnoseResultLabelWidth:390
    property int diagnoseResolutionWidth:clientWidth-diagnoseResultLabelWidth
    property bool exception:false
    property int eachLabelHeight: 14//28
    id:dialog

    onVisibleChanged: {
        if(!visible){
            exception = false;
        }
    }

    flags: Qt.SubWindow | Qt.FramelessWindowHint;
    modality: Qt.ApplicationModal
    color:translucent
    RectangularGlow{
        anchors.fill: main;
        glowRadius: 5;
        cornerRadius:10;
        spread: 0.4;color:"#7F000000";
    }
    Rectangle{
        id:main;
        width:clientWidth;height: clientHeight
        radius: 5;
        anchors.centerIn: parent;
        Rectangle{
           id:titleLabel
           width:clientWidth;height: 40
           radius: 5;
           //color:blue
           MyLabel{
               text:title
               anchors.left: parent.left;anchors.leftMargin: leftMargin
               anchors.top: parent.top;anchors.topMargin: topMargin;
               font.pixelSize: 18;
            }
        }

        ImageButton {//Close Button
            id: btnClose
            anchors.right: parent.right;anchors.rightMargin: rightMargin;
            anchors.top: parent.top;anchors.topMargin: topMargin;
            image_normal: "qrc:/resource/quit-n.png"
            visible: showBtnClose;
            mouseArea.onClicked: {
                dialog.close();
            }
        }
        Rectangle{
            id:clientArea;
            width: parent.width; height:parent.height-mMargin;
            anchors.top: parent.top;anchors.topMargin: mMargin;
            color:translucent
        }

        Rectangle{
            id:diagnoseItem
            width: parent.width; height:30;
            anchors.top: titleLabel.bottom
            color:translucent
            //radius: 5;
            MyLabel{
                id:diagnoseLabel;
                text: qsTr("诊断项")
                anchors.left: parent.left;anchors.leftMargin: 40
                anchors.verticalCenter: parent.verticalCenter
                font.family: webFont_bold.name;
            }
            MyLabel{
                id:diagnoseResultLabel;
                text: qsTr("诊断结果")
                anchors.left: parent.left;anchors.leftMargin: diagnoseLabelWidth
                anchors.verticalCenter: parent.verticalCenter
                font.family: webFont_bold.name;
            }
            MyLabel{
                id:diagnoseResolutionLabel;
                text: qsTr("解决方案")
                anchors.left: parent.left;anchors.leftMargin: diagnoseResultLabelWidth
                anchors.verticalCenter: parent.verticalCenter
                font.family: webFont_bold.name;
            }

        }
        Rectangle{//line
             id:line;
             width:parent.width-80;height:1;
             color: white;
             anchors.left: parent.left;anchors.leftMargin: 40
             anchors.top: diagnoseItem.bottom;
         }

        Rectangle{
            width:parent.width
            height: parent.width;
            color:translucent
            MyLabel{
                id:emptyLabel;
                y:220;
                anchors.horizontalCenter: parent.horizontalCenter;
                text: qsTr("未知原因,请通过以下方法解决：")
                font.pixelSize:18
                visible: !exception&&diagnose.height<eachLabelHeight
                //height: visible?contentHeight:0
            }
            MyLink{
                id:supportLabel;
                anchors.top: emptyLabel.bottom;anchors.topMargin:topMargin;
                anchors.horizontalCenter: parent.horizontalCenter;
                visible: emptyLabel.visible
                title: qsTr("官方技术支持: ");
                url:qsTr("https://www.pimaxvr.com/support/");
            }
        }
        MyScrollView{
            width: parent.width;
            height:diagnose.height;
            anchors.left: parent.left;
            anchors.top: diagnoseItem.bottom;
            Rectangle{
                    id: diagnose;
                    width:diagnoseLabelWidth
                    height: dfuDriverLabel.y+dfuDriverLabel.height;
                    anchors.left:parent.left;anchors.leftMargin: 40
                    color:translucent
                    MyLabel{
                        id:appLabel;
                        anchors.top: parent.top;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("应用完整性检测: ")
                        visible: diagnoseHandler.appComponentMissed?exception=true:false
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:serviceLabel;
                        anchors.top: appLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("服务检测:")
                        visible: (!diagnoseHandler.serviceRunning)?(exception=true):false
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:sp1Label;
                        anchors.top: serviceLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("SP1补丁:")
                        visible: diagnoseHandler.sp1_missed?exception=true:false
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:gpuDriverLabel;
                        anchors.top: sp1Label.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("显卡驱动安装:")
                        visible: !diagnoseHandler.gpuDriverInstalled?exception=true:false
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:usbLabel;
                        anchors.top: gpuDriverLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("USB检测:")
                        visible: (!diagnoseHandler.usbPlugin && diagnoseHandler.serviceRunning)?exception=true:false
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:gpuLabel;
                        anchors.top: usbLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("显卡驱动检测:")
                        visible: (!diagnoseHandler.gpuVersionValid && diagnoseHandler.usbPlugin)?exception=true:false
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:videoLabel;
                        anchors.top: gpuLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("HDMI/DP检测:")
                        visible: (!diagnoseHandler.videoPlugin && diagnoseHandler.serviceRunning)?exception=true:false
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:dfuDriverLabel;
                        anchors.top: videoLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("DFU驱动检测:")
                        visible: (!diagnoseHandler.dfu_driver_installed && diagnoseHandler.serviceRunning)?exception=true:false
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
            }
            Rectangle{
                    id: diagnoseResult;
                    width:diagnoseResultLabelWidth
                    height: diagnose.height;
                    anchors.left: parent.left;anchors.leftMargin: diagnoseLabelWidth;
                    color:translucent
                    MyLabel{
                        id:appResultLabel;
                        visible: appLabel.visible
                        height: visible?contentHeight:0
                        y:appLabel.y
                        text: diagnoseHandler.appComponentMissed?qsTr("应用组件不完整"):qsTr("通过");
                        color: diagnoseHandler.appComponentMissed?red:black;
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:serviceResultLabel;
                        visible: serviceLabel.visible
                        height: visible?contentHeight:0
                        y: serviceLabel.y
                        text: diagnoseHandler.serviceRunning? qsTr("服务已启动"):qsTr("服务未开启")
                        color:diagnoseHandler.serviceRunning?black:red;
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:sp1ResultLabel;
                        visible: sp1Label.visible
                        height: visible?contentHeight:0
                        y: sp1Label.y
                        text: diagnoseHandler.sp1_missed? qsTr("SP1补丁缺失"):qsTr("通过")
                        color:diagnoseHandler.sp1_missed?red:black;
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:gpuDriverResultLabel;
                        visible: gpuDriverLabel.visible
                        height: visible?contentHeight:0
                        y: gpuDriverLabel.y
                        text: diagnoseHandler.gpuDriverInstalled? qsTr("已安装"):qsTr("未安装")
                        color:diagnoseHandler.gpuDriverInstalled?black:red;
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:usbResultLabel;
                        visible: usbLabel.visible
                        height: visible?contentHeight:0
                        y: usbLabel.y
                        text: diagnoseHandler.usbPlugin? qsTr("已连接"):qsTr("未连接")
                        color:diagnoseHandler.usbPlugin?black:red;
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:gpuResultLabel;
                        visible: gpuLabel.visible
                        height: visible?contentHeight:0
                        y: gpuLabel.y
                        text: qsTr("此显卡驱动版本与头盔不匹配")//getGpuList()
                        color:visible?red:black;
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:videoResultLabel;
                        visible: videoLabel.visible
                        height: visible?contentHeight:0
                        y: videoLabel.y
                        text: diagnoseHandler.videoPlugin? qsTr("已连接"):qsTr("未连接")
                        color:diagnoseHandler.videoPlugin?black:red;
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:dfuDriverResultLabel;
                        visible: dfuDriverLabel.visible
                        height: visible?contentHeight:0
                        y: dfuDriverLabel.y
                        text: diagnoseHandler.dfu_driver_installed? qsTr("已安装"):qsTr("未安装")
                        color:diagnoseHandler.dfu_driver_installed?black:red;
                        font.pixelSize:12
                    }

            }
            Rectangle{
                    id: resolution;
                    height: diagnose.height;
                    anchors.left: parent.left;anchors.leftMargin: diagnoseResultLabelWidth;
                    color:translucent
                    MyLink{
                        id:appResolutionLabel;
                        visible: appLabel.visible
                        y:appLabel.y
                        urlTitle: diagnoseHandler.appComponentMissed?qsTr("重新安装应用程序"):qsTr("");
                        fontSize:12
                    }
                    MyLink{
                        id:serviceResolutionLabel;
                        visible: serviceLabel.visible
                        y: serviceLabel.y
                        urlTitle: diagnoseHandler.serviceRunning? qsTr(""):qsTr("启动服务")
                        fontSize:12
                        mouseArea.onClicked: {
                            diagnoseHandler.startService();
                        }
                    }
                    MyLink{
                        id:sp1ResolutionLabel;
                        visible: sp1Label.visible
                        y: sp1Label.y
                        urlTitle: diagnoseHandler.sp1_missed? qsTr("下载并安装SP1补丁"):qsTr("")
                        fontSize:12
                    }
                    MyLink{
                        id:gpuDriverResolutionLabel;
                        visible: gpuDriverResultLabel.visible
                        y: gpuDriverResultLabel.y
                        urlTitle: !diagnoseHandler.gpuDriverInstalled? qsTr("请安装合适的显卡驱动"):qsTr("")
                        fontSize:12
                    }
                    MyLink{
                        id:usbResolutionLabel;
                        visible: usbLabel.visible
                        y: usbLabel.y
                        urlTitle: diagnoseHandler.usbPlugin? qsTr(""):qsTr("请连接头盔USB")
                        fontSize:12
                    }
                    MyLink{
                        id:gpuResolutionLabel;
                        visible: gpuLabel.visible
                        y: gpuLabel.y
                        urlTitle: qsTr("请安装合适的显卡驱动")
                        url:"https://www.pimaxvr.com/driver_support/";
                        fontSize:12
                    }
                    MyLink{
                        id:videoResolutionLabel;
                        visible: videoLabel.visible
                        y: videoLabel.y
                        urlTitle: diagnoseHandler.videoPlugin? qsTr(""):qsTr("请连接头盔HDMI/DP线")
                        fontSize:12
                    }
                    MyLink{
                        id:dfuDriverResolutionLabel;
                        visible: dfuDriverLabel.visible
                        y: dfuDriverLabel.y
                        urlTitle: diagnoseHandler.dfu_driver_installed? qsTr(""):qsTr("安装Dfu驱动")
                        fontSize:12
                        mouseArea.onClicked: {
                            diagnoseHandler.LaunchDfuDriverSetupPackage();
                        }
                    }

            }
        }

    }

    function getGpuList()
    {
        var str = "";
        for(var i = 0; i < diagnoseHandler.gpuSum; i++){
            str += diagnoseHandler.getGpuName(i);
            str += "\n(";
            str += diagnoseHandler.getGpuVersion(i);
            console.log("diagnose>>>>>>>>>>>>>>>",str);
            str += ")\n";
        }
        return str;
    }
}
