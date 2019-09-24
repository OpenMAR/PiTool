import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import pimaxvr.Setting 1.0
import QtQuick.Dialogs 1.2
import pimaxvr.PiService 1.0

MyScrollView {
    property int parent_width : parent.width
    property int parent_height: parent.height

    Rectangle{
        width:parent_width;height: parent_height;
        color:translucent
        visible: !piService.connect
        enabled: !piService.connect
        Image{
            id:imageNoResource;
            anchors.top: parent.top;anchors.topMargin: 35;
            anchors.horizontalCenter: parent.horizontalCenter;
            source: "qrc:/resource/none.png";

        }
        MyLabel{
            id:labelNoResource;
            anchors.top: imageNoResource.bottom
            anchors.topMargin: 50;
            anchors.horizontalCenter: parent.horizontalCenter;
            text:qsTr("设备未连接")
        }
    }
    Loader{
        id:spaceCalibrate
    }

    Rectangle {
        width: parent_width;height: parent_height;
        color: translucent;
        visible:piService.connect

        MyCheckBox {
            id:smartFillFrame
            text:qsTr("开启智能补帧")
            anchors.top: parent.top; anchors.topMargin: 5
            checked: piService.smartSmoothingEnable;
            onCheckedChanged: {
                if((checked != piService.smartSmoothingEnable) && !piService.setSmartSmoothingEnable(checked)){

                }
            }
        }
        MyExclamatoryMark{
            visible: smartFillFrame.visible
            displayTitle:qsTr("开启智能补帧")
            displayNote:qsTr("通过对画面以及头显追踪数据的预测\n以智能补帧的方式将刷新率提高。")
            anchors.left: smartFillFrame.right;anchors.leftMargin: 15
            anchors.top: smartFillFrame.top;anchors.topMargin: -5
            mWidth:settingModel.mLanguage==0?255:360
            mHeight:85
        }

        MyCheckBox {
            id:foveated
            text: qsTr("开启固定注视点渲染")
            visible: piService.foveatedSupport
            height: visible?21:0
            anchors.top: smartFillFrame.bottom; anchors.topMargin: visible?18:0
            checked: piService.foveatedEnalbe;
            onCheckedChanged: {
                if((checked != piService.foveatedEnable) && !piService.setFoveatedEnable(checked)){

                }
            }
        }
        MyComboBox {
            id: foveatedcom
            currentIndex: piService.foveatedLevel
            model: [qsTr("性能优先"),qsTr("平衡"),qsTr("画质优先")]
            visible: foveated.visible
            height: visible?30:0
            width: 103
            borderColor:white
            anchors.top: foveated.bottom;anchors.topMargin: visible?14:0
            onCurrentIndexChanged: {
                if(visible&&(currentIndex!=piService.foveatedLevel)){
                    piService.setFoveatedLevel(currentIndex);
                }
            }
        }

        MyCheckBox {
            id: compParaPro
            text: qsTr("兼容使用平行投影")
            anchors.top: foveatedcom.bottom; anchors.topMargin: visible?18:0
            checked:piService.compatibleSteamvrUsingParallel;
            visible:piService.connect&&piService.modeType==0&&piService.hmd_name.indexOf("8K")>0
            onCheckedChanged: {
                piService.setCompatibleSteamvrUsingParallel(checked);
            }
        }
        MyExclamatoryMark{
            visible: compParaPro.visible
            displayTitle:qsTr("兼容使用平行投影的游戏")
            displayNote:qsTr("如发现某个游戏在头盔中画面无法\n重合。请尝试勾选此项，但这有可\n能对游戏的性能造成影响。")
            anchors.left: compParaPro.right;anchors.leftMargin: 15
            anchors.top: compParaPro.top;anchors.topMargin: -5
            mWidth:settingModel.mLanguage==0?220:360
            mHeight:135

            Image {
                id: myImage
                visible: parent.note.visible
                anchors.bottom: parent.note.bottom;
                anchors.horizontalCenter: parent.note.horizontalCenter
                width: 100
                height: 50
                source: "qrc:/resource/face_not_coincide.jpg"
            }

        }
        MyCheckBox {
            id: supperViveGame
            text: qsTr("兼容Vive独占游戏")
            anchors.top: compParaPro.bottom; anchors.topMargin: visible?18:0
            checked:piService.m_supportViveGanme//piService.compatibleSteamvrUsingParallel;
            //visible:true
            onCheckedChanged: {
                piService.setSupportViveGame(checked);
            }
        }
        MyLabel {
            id: labelFov1
            visible: parent.visible&&piService.modeType==0&&piService.hmd_name.indexOf("8K")>0
            anchors.top: supperViveGame.bottom;anchors.topMargin: visible?18:0
            anchors.left: compParaPro.left;
            height: visible?21:0
            text: qsTr("视场角(需要重启VR应用)")
        }
        MyComboBox {
            id: comboFov1
            currentIndex: piService.fovType
            model: [qsTr("超大"),qsTr("普通"),qsTr("略小")]
            visible: labelFov1.visible
            height: visible?30:0
            width: 103
            borderColor:white
            anchors.top: labelFov1.bottom;anchors.topMargin: visible?14:0
            onCurrentIndexChanged: {
                if(visible&&(currentIndex!=piService.fovType)){
                    piService.setFovType(currentIndex);
                }
            }
        }
        MyLabel {
            id: labelRefresh
            visible: piService.connect&&piService.modeType==0&&piService.hmd_name.indexOf("8K")>0&&piService.refreshCounts>1
            anchors.left: labelFov1.left
            anchors.top: comboFov1.bottom;anchors.topMargin: visible?18:0
            height: visible?21:0
            text: qsTr("刷新率")
        }
        MyComboBox {
            id: comboRefresh
            currentIndex: piService.refreshIndex
            model: piService.refreshValues //[qsTr("60"),qsTr("72"),qsTr("90")]
            visible: labelRefresh.visible
            height: visible?30:0
            width: 103
            borderColor:white
            anchors.left: labelFov1.left
            anchors.top: labelRefresh.bottom;anchors.topMargin: visible?14:0
            onCurrentIndexChanged: {
                if(visible&&(currentIndex!=piService.refreshIndex)&&!piService.setRefreshByIndex(currentIndex)){
                    comboBright.currentIndex = piService.refreshIndex;
                }
            }
         }
    }

}
