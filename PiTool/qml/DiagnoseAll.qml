import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import pimaxvr.PiService 1.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Window {
    property int mMargin: 30;
    property int leftMargin: 20;
    property int topMargin: 20;
    property int rightMargin: 10;
    property int fontSize: 14;
    property int clientWidth: 700;
    property int clientHeight: 500;
    property var title:qsTr("环境诊断")
    property bool showBtnClose: true;
    property alias btnClose:btnClose.mouseArea;
    property int diagnoseLabelWidth:210
    property int diagnoseResultLabelWidth:390
    property int diagnoseResolutionWidth:clientWidth-diagnoseResultLabelWidth
    id:diagnoseAllDialog

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
               //bold:true
               anchors.left: parent.left;anchors.leftMargin: leftMargin
               anchors.top: parent.top;anchors.topMargin: 10;
               font.pixelSize: 18;
            }
        }

        ImageButton {//Close Button
            id: btnClose
            anchors.right: parent.right;anchors.rightMargin: rightMargin;
            anchors.top: parent.top;anchors.topMargin: 10;
            image_normal: "qrc:/resource/quit-n.png"
            visible: showBtnClose;
            mouseArea.onClicked: {
                diagnoseAllDialog.close();
            }
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

        MyScrollView{
            width: parent.width-40;
            height:parent.height-y;
            anchors.left: parent.left;
            anchors.top: diagnoseItem.bottom;
            Rectangle{
                width: parent.width;
                height:diagnose.height;
                Rectangle{
                    id: diagnose;
                    width:diagnoseLabelWidth
                    height: steamvrLabel.y+steamvrLabel.height;
                    anchors.left:parent.left;anchors.leftMargin: 40
                    color:translucent
                    MyLabel{
                        id:appLabel;
                        anchors.top: parent.top;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("应用完整性检测: ")
                        visible: true
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:serviceLabel;
                        anchors.top: appLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("服务检测:")
                        visible: true
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:sp1Label;
                        anchors.top: serviceLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("SP1补丁:")
                        visible: true
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:patchLabel;
                        anchors.top: sp1Label.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("游戏补丁检测：");
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:gpuDriverLabel;
                        anchors.top: patchLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("显卡驱动安装:")
                        visible: false
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:usbLabel;
                        //anchors.top: gpuDriverLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        y:patchResultLabel.y+patchResultLabel.contentHeight+topMargin
                        text: qsTr("USB检测:")
                        visible: true
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:usbPortLabel;
                        anchors.top: usbLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("USB3.0接口：");
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:gpuLabel;
                        anchors.top: usbPortLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("显卡驱动检测:")
                        visible: true
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:videoLabel;
                        //anchors.top: gpuLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        y:gpuResultLabel.y+gpuResultLabel.contentHeight+topMargin
                        text: qsTr("HDMI/DP检测:")
                        visible: true
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:videoTypeLabel;
                        //anchors.top: gpuLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        anchors.top: videoLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("视频接口协议: ")
                        visible: diagnoseHandler.videoPlugin
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:dxLabel;
                        anchors.top: videoTypeLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("DX检测: ")
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:vcLabel;
                        anchors.top: dxLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("VC检测: ")
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:osLabel;
                        anchors.top: vcLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("操作系统：");
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:pcTypeLabel;
                        anchors.top: osLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("电脑类型：");
                        height: visible?contentHeight:0
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:steamvrLabel;
                        anchors.top: pcTypeLabel.bottom;anchors.topMargin:visible?topMargin:0;
                        text: qsTr("SteamVR检测：");
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
                        id:patchResultLabel;
                        visible: patchLabel.visible
                        height: visible?contentHeight:0
                        y: patchLabel.y
                        text: getPatchList();
                        font.pixelSize:12
                        color:diagnoseHandler.patchMissedSum > 0?red:black
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
                        id:usbPortResultLabel;
                        visible: usbPortLabel.visible
                        height: visible?contentHeight:0
                        y: usbPortLabel.y
                        text: diagnoseHandler.usb3_0?qsTr("通过"):qsTr("没有检测到USB3.0接口");
                        color:diagnoseHandler.usb3_0?black:red;
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:gpuResultLabel;
                        visible: gpuLabel.visible
                        height: visible?contentHeight:0
                        y: gpuLabel.y
                        text: getGpuList()//!diagnoseHandler.gpuVersionValid?qsTr("此显卡驱动版本与头盔不匹配"):qsTr("驱动版本正常")//
                        color:(!diagnoseHandler.gpuDriverInstalled || !diagnoseHandler.gpuVersionValid)?red:black;
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:videoResultLabel;
                        visible: videoLabel.visible
                        height: visible?contentHeight:0
                        y: videoLabel.y
                        text: diagnoseHandler.videoPlugin? (diagnoseHandler.integratedGraphicsPlugin?qsTr("连接到集显"):qsTr("已连接")):qsTr("未连接")
                        color:diagnoseHandler.videoPlugin?black:red;
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:videoTypeResultLabel;
                        visible: videoTypeLabel.visible
                        height: visible?contentHeight:0
                        y: videoTypeLabel.y;
                        font.pixelSize:12
                        text://getVideoOutputType()
                              if(diagnoseHandler.videoOutputType == 1){
                                  return qsTr("VGA");
                              }else if(diagnoseHandler.videoOutputType == 2){
                                  return qsTr("DVI");
                              }else if(diagnoseHandler.videoOutputType == 3){
                                  return qsTr("DVI");
                              }else if(diagnoseHandler.videoOutputType == 4){
                                  return qsTr("HDMI");
                              }else if(diagnoseHandler.videoOutputType == 5){
                                  return qsTr("DP");
                              }else{
                                  return qsTr("无法识别");
                              }
                        color: black;
                    }
                    MyLabel{
                        id:dxResultLabel;
                        visible: dxLabel.visible
                        height: visible?contentHeight:0
                        y: dxLabel.y;
                        text: diagnoseHandler.dxFileMissed?qsTr("DX组件缺失"):qsTr("通过");
                        color: diagnoseHandler.dxFileMissed?red:black;
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:vcResultLabel;
                        visible: vcLabel.visible
                        height: visible?contentHeight:0
                        y: vcLabel.y;
                        text: diagnoseHandler.vcMissed?qsTr("VC运行库组件缺失"):qsTr("通过");
                        color: diagnoseHandler.vcMissed?red:black;
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:osResultLabel;
                        visible: osLabel.visible
                        height: visible?contentHeight:0
                        y: osLabel.y
                        text: diagnoseHandler.os_version;
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:pcTypeResultLabel;
                        visible: pcTypeLabel.visible
                        height: visible?contentHeight:0
                        y: pcTypeLabel.y
                        text: diagnoseHandler.isNoteBook?qsTr("笔记本"):qsTr("台式机");
                        font.pixelSize:12
                    }
                    MyLabel{
                        id:steamvrResultLabel;
                        visible: steamvrLabel.visible
                        height: visible?contentHeight:0
                        y: steamvrLabel.y
                        text: diagnoseHandler.steamvrValid?qsTr("通过"):qsTr("存在异常");
                        color:diagnoseHandler.steamvrValid?black:red;
                        font.pixelSize:12
                    }

            }
                Rectangle{
                    id: resolution;
                    height: diagnose.height;
                    anchors.left: parent.left;anchors.leftMargin: diagnoseResultLabelWidth;
                    color:translucent
                    MyLabel{
                        id:appResolutionLabel;
                        visible: appLabel.visible
                        font.pixelSize:12
                        y:appLabel.y
                        text: diagnoseHandler.appComponentMissed?qsTr("请重新安装应用程序"):qsTr("");
                    }
                    MyLink{
                        id:serviceResolutionLabel;
                        visible: serviceLabel.visible
                        fontSize:12
                        y: serviceLabel.y
                        urlTitle: diagnoseHandler.serviceRunning? qsTr(""):qsTr("启动服务")
                        mouseArea.onClicked: {
                            diagnoseHandler.startService();
                        }
                    }
                    MyLabel{
                        id:sp1ResolutionLabel;
                        visible: sp1Label.visible
                        font.pixelSize:12
                        y: sp1Label.y
                        text: diagnoseHandler.sp1_missed? qsTr("请升级操作系统为WIn7 SP1或以上"):qsTr("")
                    }
                    MyLabel{
                        id:patchResolutionLabel;
                        visible: patchLabel.visible
                        y: patchLabel.y
                        text: diagnoseHandler.patchMissedSum > 0?qsTr("请安装缺失补丁"):qsTr("");
                        font.pixelSize:12
                    }
                    MyLink{
                        id:gpuDriverResolutionLabel;
                        visible: gpuDriverResultLabel.visible
                        fontSize:12
                        y: gpuDriverResultLabel.y
                        urlTitle: !diagnoseHandler.gpuDriverInstalled? qsTr("请安装合适的显卡驱动"):qsTr("")
                        url:"https://www.pimaxvr.com/driver_support/";
                    }
                    MyLabel{
                        id:usbResolutionLabel;
                        visible: usbLabel.visible
                        font.pixelSize:12
                        y: usbLabel.y
                        text: diagnoseHandler.usbPlugin? qsTr(""):qsTr("请连接头盔USB")
                    }
                    MyLink{
                        id:usbPortResolutionLabel;
                        visible: usbPortLabel.visible
                        fontSize:12
                        y: usbPortLabel.y
                    }
                    MyLink{
                        id:gpuResolutionLabel;
                        visible: gpuLabel.visible
                        fontSize:12
                        y: gpuLabel.y
                        urlTitle: diagnoseHandler.gpuOnlyIntelCard?
                                      qsTr("集成显卡影响性能,请升级配置"):(!diagnoseHandler.gpuVersionValid?qsTr("请安装合适的显卡驱动"):qsTr(""))
                    }
                    MyLabel{
                        id:videoResolutionLabel;
                        visible: videoLabel.visible
                        font.pixelSize:12
                        y: videoLabel.y
                        text: diagnoseHandler.videoPlugin? qsTr(""):qsTr("请连接头盔HDMI/DP线")
                    }
                    MyLink{
                        id:videoTypeResolutionLabel;
                        visible: videoTypeLabel.visible
                        fontSize:12
                        y: videoTypeLabel.y;
                        urlTitle: qsTr("");
                    }
                    MyLink{
                        id:dxResolutionLabel;
                        visible: dxLabel.visible
                        fontSize:12
                        y: dxLabel.y;
                        urlTitle: diagnoseHandler.dxFileMissed?qsTr("进行DX修复"):qsTr("");
                        url:diagnoseHandler.dxFileMissed?"http://pisetup.pimaxvr.com/dx11.zip":qsTr("")
                    }
                    MyLink{
                        id:vcResolutionLabel;
                        visible: vcLabel.visible
                        fontSize:12
                        y: vcLabel.y;
                        urlTitle: diagnoseHandler.vcMissed?qsTr("安装VC运行库"):qsTr("");
                        url:diagnoseHandler.vcMissed?"http://pisetup.pimaxvr.com/vc.zip":qsTr("")
                    }
                    MyLink{
                        id:osResolutionLabel;
                        visible: osLabel.visible
                        fontSize:12
                        y: osLabel.y
                    }
                    MyLink{
                        id:pcTypeResolutionLabel;
                        visible: pcTypeLabel.visible
                        fontSize:12
                        y: pcTypeLabel.y
                    }
                    MyLabel{
                        id:steamvrResolutionLabel;
                        visible: steamvrLabel.visible
                        y: steamvrLabel.y
                        text: diagnoseHandler.steamvrValid?qsTr(""):qsTr("建议重新安装SteamVR");
                        font.pixelSize:12
                    }

            }
            }
        }

    }

    function getGpuList()
    {
        var str = "";
        if(!diagnoseHandler.gpuDriverInstalled){
            str += qsTr("未检测到显卡驱动");
            return str;
        }

        for(var i = 0; i < diagnoseHandler.gpuSum; i++){
            str += diagnoseHandler.getGpuName(i);
            str += "\n(";
            str += diagnoseHandler.getGpuVersion(i);
            str += ")\n";
        }
        return str;
    }
    function getPatchList()
    {
        var str = "";
        if(diagnoseHandler.patchMissedSum <= 0){
            str += qsTr("通过");
            return str;
        }
        for(var i = 0; i < diagnoseHandler.patchMissedSum; i++){
            str += "KB" + diagnoseHandler.patchList(i);
            str += "\n";
        }
        return str;
    }
    function getVideoOutputType()
    {
        var str = "";
        if(diagnoseHandler.videoOutputType == 1){
            str += qsTr("VGA");
        }else if(diagnoseHandler.videoOutputType == 2){
            str +=qsTr("DVI");
        }else if(diagnoseHandler.videoOutputType == 3){
            str +=qsTr("DVI");
        }else if(diagnoseHandler.videoOutputType == 4){
            str +=qsTr("HDMI");
        }else if(diagnoseHandler.videoOutputType == 5){
            str +=qsTr("DP");
        }else{
            str +=qsTr("Unknow");
        }
        return str;
    }
}
