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
    property int lineMargin:parent.width/3
    width: parent.width;
    height:helpDiagnose.height;
    Rectangle{
       id:helpDiagnose
       width:parent_width
       height: diagnose.height;
       anchors.left: parent.left;anchors.leftMargin: mMARGIN1;
       Rectangle{
            id: diagnose;
            width:lineMargin
            height: systemDllsLabel.y+systemDllsLabel.height;
            anchors.left: parent.left;anchors.leftMargin: mMARGIN1;
            color:translucent
            MyLabel{
                id:diagnoseLabel;
                anchors.top: parent.top;anchors.topMargin:mMARGIN1;
                text: qsTr("诊断项")
            }
            MyLabel{
                id:appLabel;
                anchors.top: diagnoseLabel.bottom;anchors.topMargin:topMARGIN;
                text: qsTr("应用完整性检测: ")
            }
            MyLabel{
                id:dxLabel;
                anchors.top: appLabel.bottom;anchors.topMargin:topMARGIN;
                text: qsTr("DX检测: ")
            }
            MyLabel{
                id:vcLabel;
                anchors.top: dxLabel.bottom;anchors.topMargin:topMARGIN;
                text: qsTr("VC检测: ")
            }
            MyLabel{
                id:serviceLabel;
                y: vcResultLabel.y+vcResultLabel.height + topMARGIN;
                text: qsTr("服务检测:")
            }
            MyLabel{
                id:gpuLabel;
                anchors.top: serviceLabel.bottom;anchors.topMargin:topMARGIN;
                text: qsTr("显卡检测: ")// + diagnoseHandler.cpu;
            }
            MyText{
                id:osLabel;
                y: gpuResultLabel.y+gpuResultLabel.height + topMARGIN;
                text: qsTr("操作系统：");
            }
            MyText{
                id:pcTypeLabel;
                y: osResultLabel.y+osResultLabel.height + topMARGIN;
                text: qsTr("电脑类型：");
            }
            MyText{
                id:usbPortLabel;
                y: pcTypeResultLabel.y+pcTypeResultLabel.height + topMARGIN;
                text: qsTr("USB3.0接口：");
            }
            MyText{
                id:patchLabel;
                y: usbPortResultLabel.y+usbPortResultLabel.height + topMARGIN;
                text: qsTr("补丁检测：");
            }
            MyText{
                id:systemDllsLabel;
                y: patchResultLabel.y+patchResultLabel.height + topMARGIN;
                text: qsTr("系统组件：");
            }
        }

       Rectangle{//line
            id:line1;
            width:1;height:diagnose.height;
            color: black;
            anchors.left: parent.left;anchors.leftMargin: lineMargin;
            anchors.top: parent.top;anchors.topMargin: topMARGIN;
        }

       Rectangle{
            id: diagnoseResult;
            width:lineMargin
            height: diagnose.height;
            anchors.left: parent.left;anchors.leftMargin: lineMargin + mMARGIN1;
            color:translucent
            MyLabel{
                id:diagnoseResultLabel;
                anchors.top: parent.top;anchors.topMargin:mMARGIN1;
                text: qsTr("诊断结果")
            }
            MyLabel{
                id:appResultLabel;
                y:appLabel.y
                text: diagnoseHandler.appComponentMissed?qsTr("应用组件不完整"):qsTr("通过");
                color: diagnoseHandler.appComponentMissed?red:black;
            }
            MyLabel{
                id:dxResultLabel;
                y: dxLabel.y;
                text: diagnoseHandler.dxFileMissed?qsTr("DX组件缺失，建议进行DX修复"):qsTr("通过");
                color: diagnoseHandler.dxFileMissed?red:black;
            }
            MyLabel{
                id:vcResultLabel;
                y: vcLabel.y;
                text: getVcList();
                color: diagnoseHandler.vcMissed?red:black;
            }
            MyLabel{
                id:serviceResultLabel;
                y: serviceLabel.y
                text: diagnoseHandler.serviceRunning? qsTr("服务已启动"):qsTr("服务未开启")
                color:diagnoseHandler.serviceRunning?black:red;
            }
            MyLabel{
                id:gpuResultLabel;
                y: gpuLabel.y
                text: getGpuList()
            }
            MyText{
                id:osResultLabel;
                y: osLabel.y
                text: diagnoseHandler.os_version;
            }
            MyText{
                id:pcTypeResultLabel;
                y: pcTypeLabel.y
                text: diagnoseHandler.isNoteBook?qsTr("笔记本"):qsTr("台式机");
            }
            MyText{
                id:usbPortResultLabel;
                y: usbPortLabel.y
                text: diagnoseHandler.usb3_0?qsTr("通过"):qsTr("没有检测到USB3.0接口");
                color:diagnoseHandler.usb3_0?black:red;
            }
            MyText{
                id:patchResultLabel;
                y: patchLabel.y
                text: getPatchList();
            }
            MyText{
                id:systemDllsResultLabel;
                y: systemDllsLabel.y
                text: diagnoseHandler.missedDlls? qsTr("系统关键组件缺失"):qsTr("通过");
                color:diagnoseHandler.missedDlls?red:black
            }
        }

       Rectangle{//line
            id:line2;
            width:1;height:diagnose.height;
            color: black;
            anchors.left: parent.left;anchors.leftMargin: lineMargin*2;
            anchors.top: parent.top;anchors.topMargin: topMARGIN;
        }

       Rectangle{
            id: resolution;
            width:lineMargin
            height: diagnose.height;
            anchors.left: parent.left;anchors.leftMargin: lineMargin*2+mMARGIN1;
            color:translucent
            MyLabel{
                id:resolutionLabel;
                anchors.top: parent.top;anchors.topMargin:mMARGIN1;
                text: qsTr("解决方案")
            }
        }

    }

    function getVcList(){
        var str = "";
        if(!diagnoseHandler.vcMissed){
            str += qsTr("通过");
            return str;
        }

        if(diagnoseHandler.vcMissedCount == 0){
            str += qsTr("VC运行库缺失");
            return str;
        }

        for(var i = 0; i < diagnoseHandler.vcMissedCount; i++){
            str += diagnoseHandler.vcList(i);
            str += "\n";
        }
        return str;
    }

    function getGpuList()
    {
        var str = "";
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
            str += diagnoseHandler.patchList(i);
            str += "\n";
        }
        return str;
    }
}

