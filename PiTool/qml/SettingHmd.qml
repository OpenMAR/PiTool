import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import pimaxvr.PiService 1.0
MyScrollView{
    property int parent_width: parent.width;
    property int parent_height:parent.height
    property int btnWidth: 150;
    property int btnWidthRestore: 70;
    property var topMARGIN:35;
    property var rightMARGIN: 30;
    property var leftMARGIN: 40;
    property var mMARGIN:10;
    Rectangle{
        width:parent_width;height: parent_height;
        color:translucent
        visible: !piService.connect
        enabled: !piService.connect
        Image{
            id:imageNoResource;
            anchors.top: parent.top;anchors.topMargin: topMARGIN;
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
    Rectangle{
        width: parent_width;
        height: btnRestore.y+btnRestore.height+topMARGIN;
        visible: piService.connect

        //固件版本
        MyLabel{
            id:labelVersion
            visible: piService.connect
            text:qsTr("头盔固件版本:")
            anchors.top: parent.top;anchors.topMargin: visible?5:0
            //anchors.left: parent.left;anchors.leftMargin: leftMARGIN;
        }
        MyLabel{
            id:textVersion
            visible: piService.connect
            text:"V"+piService.fmVersion
            anchors.left: labelVersion.right;anchors.leftMargin: 5;
            anchors.top:labelVersion.top
        }
        TextButton{
            id:updateCheckButton
            text: qsTr("检查更新")
            visible: piService.connect
            width: btnWidth
            anchors.left: textVersion.right;anchors.leftMargin: 20;
            anchors.verticalCenter: labelVersion.verticalCenter
            RedPoint{
                visible: versionModel.firmwareVersion.upgradeFlag>0;
            }
            mouseArea.onClicked: {
                if(piService.noDfuDriver){
                    showNoDfuDriverDialog();
                }else{
                    showVersionCheckingDFUDialog();
                }
            }
        }
        MyCheckBox {
            id: lighthouseBox
            anchors.top: labelVersion.bottom;anchors.topMargin: visible?20:0
            text: qsTr("开启Lighthouse定位方案")
            checked:piService.lighthouseEnabled;
            onCheckedChanged: {
                if(visible&&(checked!=piService.lighthouseEnabled)){
                    piService.setLighthouseEnabled(checked);
                }
            }
            visible: piService.connect&&piService.modeType==0&&piService.hmd_name.indexOf("8K")>0
            height: visible?fontSize:0
        }
        MyCheckBox {
            id: audioBox
            anchors.top: lighthouseBox.bottom;anchors.topMargin: visible?20:0
            text: qsTr("声音默认从小派输出")
            checked:piService.audioPimax;
            onCheckedChanged: {
                //if(visible&&(checked!=piService.lighthouseEnabled)){
                piService.writeAudioPimax(checked);
            }
            visible: piService.connect&&piService.modeType==0&&piService.audioCtrl
            height: visible?fontSize:0
        }
        MyLabel{
            id:displayLabel
            text:qsTr("显示")
            anchors.top:audioBox.bottom;anchors.topMargin: 20
        }
        Rectangle{//line
            id:displayLine;
            width: parent.width;height: 1;
            color: white;
            anchors.left: displayLabel.right;anchors.leftMargin: 5
            anchors.right: parent.right;anchors.rightMargin: 20
            anchors.verticalCenter: displayLabel.verticalCenter
        }
        MyCheckBox {
            id: hiddenArea
            anchors.left: displayLabel.left
            anchors.top: displayLabel.bottom;anchors.topMargin: visible?25:0
            text: qsTr("开启隐藏区域遮罩(需要重新启动SteamVR)")
            checked: piService.hiddenAreanEnabled
            onCheckedChanged: {
                    if(visible&&(checked!=piService.hiddenAreanEnabled)){
                        piService.EnablehiddenArea(checked);
                    }
            }
            visible: piService.modeType==0&&piService.connect
            height: visible?fontSize:0
        }
        MyExclamatoryMark{
            visible: hiddenArea.visible
            displayTitle:qsTr("开启隐藏区域遮罩")
            displayNote:qsTr("这个功能勾选会使部分游戏有更好\n的性能，但是个别游戏可能会有显\n示异常问题")
            anchors.left: hiddenArea.right;anchors.leftMargin: 15
            anchors.top: hiddenArea.top;anchors.topMargin: -5
            mWidth:settingModel.mLanguage==0?258:258 //178:200
            mHeight:settingModel.mLanguage==0?165:165 //85:115

            Image {
                id: myImage1
                visible:parent.note.visible
                anchors.bottom: parent.note.bottom; anchors.bottomMargin: 20
                anchors.left: parent.note.left; anchors.leftMargin: 15
                width: 100; height: 50
                source: "qrc:/resource/normal.jpg"
                z:parent.note.z
            }

            Text {
                visible: parent.note.visible
                anchors.top: myImage1.bottom
                anchors.horizontalCenter: myImage1.horizontalCenter
                text: qsTr("正常")
                z:parent.note.z
            }
            Image {
                visible: parent.note.visible
                id:  myImage2
                source: "qrc:/resource/face_not_coincide.jpg"
                anchors.bottom: parent.note.bottom; anchors.bottomMargin: 20
                anchors.right: parent.note.right; anchors.rightMargin: 15
                width: 100; height: 50
                z:parent.note.z
            }
            Text {
                visible: parent.note.visible
                anchors.top: myImage2.bottom
                anchors.horizontalCenter: myImage2.horizontalCenter
                text: qsTr("不正常")
                z:parent.note.z
            }
        }
        //陀螺仪
        MyCheckBox {
            id: checkBox1
            anchors.left: displayLabel.left
            anchors.top: hiddenArea.bottom;anchors.topMargin: visible?25:0
            text: qsTr("锁定视角")
            checked: piService.enableGyro
            onCheckedChanged: {
                console.log("checkBox1 onCheckedChanged,",checked,piService.enableGyro);
                if(visible&&(checked!=piService.enableGyro)){
                    if(!piService.writeEnableGyro(checked)){
                        showAlertPopup(qsTr("锁定视角设置失败!"));
                        checkBox1.checked = !checked;
                    }
                }
            }
            visible: false//piService.gyroCtrl&&piService.modeType==0
            height: visible?fontSize:0
        }
        //冷色调
        MyCheckBox {
            id: checkBox2
            anchors.left: displayLabel.left
            anchors.top: checkBox1.bottom;anchors.topMargin: visible?20:0
            text: qsTr("启动冷色调")
            checked:piService.coolTone;
            onCheckedChanged: {
                if(visible&&(checked!=piService.coolTone)){
                    showSwitchConfirmDialog(2,checked);
                }
            }
            visible: piService.toneCtrl&&piService.modeType==0
            height: visible?fontSize:0
        }
        MyExclamatoryMark{
            visible: checkBox2.visible
            displayTitle:qsTr("开启冷色调")
            displayNote:qsTr("观看电影，冷色调更\n佳。")
            anchors.left: checkBox2.right;anchors.leftMargin: 15
            anchors.top: checkBox2.top;anchors.topMargin: -5
            mWidth:145
            mHeight:65
        }

        MyCheckBox {
            id: compatibleBox
            anchors.left: displayLabel.left
            anchors.top: checkBox2.bottom;anchors.topMargin: visible?20:0
            text: qsTr("兼容使用平行投影")
            checked:piService.compatibleSteamvrUsingParallel;
            onCheckedChanged: {
                //if(visible&&(checked!=piService.compatibleSteamvrUsingParallel)){
                piService.setCompatibleSteamvrUsingParallel(checked);
            }
            visible: false//piService.connect&&piService.modeType==0&&piService.hmd_name.indexOf("8K")>0
            height: visible?fontSize:0
        }
        MyExclamatoryMark{
            visible: compatibleBox.visible
            displayTitle:qsTr("兼容使用平行投影的游戏")
            displayNote:qsTr("如发现某个游戏在头盔中画面无法\n重合。请尝试勾选此项，但这有可\n能对游戏的性能造成影响。")
            anchors.left: compatibleBox.right;anchors.leftMargin: 15
            anchors.top: compatibleBox.top;anchors.topMargin: -5
            mWidth:settingModel.mLanguage==0?220:360
            mHeight:85
        }

        //Fov
        MyLabel {
            id: labelFov
            visible: false//piService.connect&&piService.modeType==0&&piService.hmd_name.indexOf("8K")>0
            anchors.left: displayLabel.left
            anchors.top: compatibleBox.bottom;anchors.topMargin: visible?20:0
            height: visible?21:0
            text: qsTr("视场角(需要重启VR应用)")
        }
        MyComboBox {
            id: comboFov
            currentIndex: piService.fovType
            model: [qsTr("超大"),qsTr("普通"),qsTr("略小")]
            visible: labelFov.visible
            height: visible?30:0
            width: 103
            borderColor:white
            anchors.left: displayLabel.left
            anchors.top: labelFov.bottom;anchors.topMargin: visible?14:0
            onCurrentIndexChanged: {
                if(visible&&(currentIndex!=piService.fovType)){
                    piService.setFovType(currentIndex);
                }
            }
        }

        //分辨率
        MyLabel {
            id: labelSolution
            visible: piService.resolutionCtrl&&piService.modeType==0
            anchors.left: displayLabel.left
            anchors.top: comboFov.bottom;anchors.topMargin: visible?20:0
            height: visible?21:0
            text: qsTr("头盔分辨率")
        }
        MyComboBox {
            id: comboSolution
            visible: piService.resolutionCtrl&&piService.modeType==0
            height: visible?30:0
            borderColor:white
            anchors.left: displayLabel.left
            anchors.top: labelSolution.bottom;anchors.topMargin: visible?14:0
            model: ListModel
            {
                id:listmodel
                ListElement{modelData:"1920*1080";cost:1}
                ListElement{modelData:"2560*1440";cost:2}
                ListElement{modelData:"2160*1200";cost:4}
                ListElement{modelData:"3840*2160";cost:8}
            }
            isBlack: true;
            delegate: ItemDelegate {
                visible: (cost & piService.resolutionCap) != 0
                height:visible?35:0;
                width: comboSolution.width
                highlighted: comboSolution.highlightedIndex == index
                contentItem: MyLabel {
                    text: modelData
                    color: comboSolution.isBlack?black:white
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle{
                    color:comboSolution.isBlack?(highlighted?"#b7b7b7":white_pure):(highlighted?"#565656":translucent)
        //            radius: 4;
                }
            }

            currentIndex: piService.resolutionIndex
            onCurrentIndexChanged: {
                if(!visible){
                    return;
                }

                var res = listmodel.get(currentIndex).cost;
                if(piService.resolution==res){
                    return;
                }

                showSwitchConfirmDialog(4,res);
            }

        }

        MyLabel {
            id: labelRefresh
            visible: false//piService.connect&&piService.modeType==0&&piService.hmd_name.indexOf("8K")>0 && piService.refreshCounts>1
            anchors.left: displayLabel.left
            anchors.top: comboSolution.bottom;anchors.topMargin: visible?20:0
            height: visible?21:0
            text: qsTr("刷新率")
        }
        MyComboBox {
            id: comboRefresh
            currentIndex: piService.refreshIndex
            model: piService.refreshValues//[qsTr("60"),qsTr("72"),qsTr("90")]
            visible: labelRefresh.visible
            height: visible?30:0
            width: 103
            borderColor:white
            anchors.left: displayLabel.left
            anchors.top: labelRefresh.bottom;anchors.topMargin: visible?14:0
            onCurrentIndexChanged: {
                if(visible&&(currentIndex!=piService.refreshIndex)&&!piService.setRefreshByIndex(currentIndex)){
                    comboBright.currentIndex = piService.refreshIndex;
                }
            }
        }


        MyLabel {
            id: labelBrightFor8k
            visible: piService.connect&&piService.modeType==0&&piService.hmd_name.indexOf("8K")>0
            anchors.left: displayLabel.left
            anchors.top: comboRefresh.bottom;anchors.topMargin: visible?20:0
            height: visible?21:0
            text: qsTr("亮度调节")
        }
        MyComboBox {
            id: comboBright
            currentIndex: piService.brightLevel
            model: [qsTr("高"),qsTr("普通"),qsTr("低")]
            visible: labelBrightFor8k.visible
            height: visible?30:0
            width: 103
            borderColor:white
            anchors.left: displayLabel.left
            anchors.top: labelBrightFor8k.bottom;anchors.topMargin: visible?14:0
            onCurrentIndexChanged: {
                if(visible&&(currentIndex!=piService.brightLevel)&&!piService.writeBrightLevel(currentIndex)){
                    showAlertPopup(qsTr("屏幕亮度设置失败!!"));
                    comboBright.currentIndex = piService.brightLevel;
                }
            }
        }

        //亮度调节
        Rectangle{
            id: bright;
            color: translucent
            anchors.top: comboBright.bottom;anchors.topMargin: visible?30:0
            anchors.left: displayLabel.left
            anchors.right:displayLine.right
            visible: piService.brightCtrl&&piService.modeType==0&&!labelBrightFor8k.visible;
            height: visible?labelBrightLow.y+labelBrightLow.height:0;

            MyLabel {
                id: labelBright
                anchors.top: parent.top.bottom;
                text: qsTr("背光亮度")
            }
            MyLabel {
                id: labelBrightLow
                anchors.verticalCenter: sliderBright.verticalCenter
                anchors.left:rectBrightValue.right;anchors.leftMargin:25;
                text: qsTr("暗")
            }
            MySlider {
                id: sliderBright
                width: 340
                anchors.top: labelBright.bottom;anchors.topMargin: 14
                anchors.left: labelBrightLow.right;anchors.leftMargin:5;
                from: piService.minBrightLevel;
                to:piService.maxBrightLevel;
                stepSize: 1;
                snapMode:Slider.SnapAlways
                value: piService.brightLevel;
                onValueChanged: {
                    if(visible&&(sliderBright.value!=piService.brightLevel)&&
                            !piService.writeBrightLevel(sliderBright.value)){
                        showAlertPopup(qsTr("屏幕亮度设置失败!!"));
                        sliderBright.value = piService.brightLevel;
                    }
                }
            }
            MyLabel {
                id: labelBrightHigh
                //anchors.top: sliderBright.bottom;anchors.topMargin: 5;
                anchors.verticalCenter: sliderBright.verticalCenter
                anchors.left: sliderBright.right;anchors.leftMargin:5;
                text: qsTr("亮")
            }

            TextButton{
                id:btnBrightRestore
                text: qsTr("默认值")
                visible: true
                width: btnWidthRestore
                fontSize:13
                anchors.right: parent.right
                anchors.verticalCenter: labelBrightHigh.verticalCenter
                mouseArea.onClicked:{
                    if(visible&&!piService.SetBrightDefault()){
                             showAlertPopup(qsTr("屏幕亮度设置失败!!"));
                        }
                    }
            }

            Rectangle{
                id: rectBrightValue;
                width: 80;
                height: labelBrightValue.height+10;
                radius: rRADIUS;
                border.color: white
                anchors.verticalCenter: sliderBright.verticalCenter
                color:white_pure
                MyLabel {
                    id: labelBrightValue
                    anchors.centerIn: parent;
                    text: parseInt(sliderBright.value*100)/100
                    color:black
                }
            }
        }
        //画质调节
        Rectangle{
            id:rectRender;
            anchors.top: bright.bottom;anchors.topMargin: visible?30:0
            anchors.left: displayLabel.left
            anchors.right:displayLine.right
            visible: piService.displayQualityCtrl&&piService.modeType==0
            height: visible?30:0

            MyLabel {
                id: labelRender
                text: qsTr("画质调节")
            }
            MyExclamatoryMark{
                displayTitle:qsTr("画质调节")
                displayNote:qsTr("请适度调节，数值过\n高会导致卡顿。")
                anchors.left: labelRender.right;anchors.leftMargin: 2
                anchors.top: labelRender.top;anchors.topMargin: -5
                mWidth:settingModel.mLanguage==0?140:245
                mHeight:65
                mtopMargin:-38
            }
//            MyLabel {
//                id: labelRenderLow
//                anchors.verticalCenter: sliderRender.verticalCenter
//                anchors.left:rectRenderValue.right;anchors.leftMargin:25;
//                text: qsTr("低")
//            }
            MySlider {
                id: sliderRender
                width: 340
                anchors.verticalCenter: labelRender.verticalCenter
                anchors.left: labelRender.left;anchors.leftMargin:140;
                from: 0.5;to:2;stepSize: 0.25;
                snapMode:Slider.SnapAlways
                value: piService.displayQuality;
                onValueChanged: {
                    if(visible&&(sliderRender.value!=piService.displayQuality)&&
                            !piService.writeDisplayQuality(sliderRender.value)){
                        showAlertPopup(qsTr("画质调节失败!"));
                        sliderRender.value = piService.displayQuality;
                    }
                }
            }
//            MyLabel {
//                id: labelRenderHigh
//                anchors.verticalCenter: sliderRender.verticalCenter
//                anchors.left: sliderRender.right;anchors.leftMargin:5;
//                text: qsTr("高")
//            }

            TextButton{
                id:btnRenderRestore
                text: qsTr("默认值")
                visible: true
                width: btnWidthRestore
                fontSize:13
                anchors.right: parent.right
                anchors.verticalCenter: sliderRender.verticalCenter
                mouseArea.onClicked:{
                        if(visible&&!piService.SetRenderDefault()){
                        showAlertPopup(qsTr("画质调节失败!"));
                        }
                    }
            }

            Rectangle{
                id: rectRenderValue;
                width: 80;
                height: labelRenderValue.height+10;
                radius: rRADIUS;
                //border.color: white
                anchors.verticalCenter: sliderRender.verticalCenter
                anchors.left: sliderRender.right; anchors.margins: 5
                color:white_pure
                MyLabel {
                    id: labelRenderValue
                    anchors.centerIn: parent;
                    text: parseInt(sliderRender.value*100)/100
                    color:black
                }
            }
        }




        //对比度
        Rectangle{
            id:contrast;
            anchors.top: rectRender.bottom;anchors.topMargin: visible?30:0
            anchors.left: displayLabel.left
            anchors.right:displayLine.right
            //visible: piService.displayQualityCtrl&&piService.modeType==0
            height: visible?100:0

            MyLabel {
                id: labelcontrast
                text: qsTr("对比度")
            }
            ImageButton2 {
                id: contrastLockButton
                image_normal: "qrc:/resource/lock2.png"
                image_pressed: "qrc:/resource/lock1.png"
                anchors.top: labelcontrast.bottom;anchors.topMargin: 19
            }

            MyLabel {
                id: contrastLeft
                text: qsTr("左")
                anchors.top: labelcontrast.bottom; anchors.topMargin: 10
                anchors.left: parent.left; anchors.leftMargin: 45
            }
            MyLabel {
                id: contrastRight
                text: qsTr("右")
                anchors.top: contrastLeft.bottom; anchors.topMargin: 15
                anchors.left: parent.left; anchors.leftMargin: 45
            }

            MySlider {
                id: sliderContrastLeft
                width: 340
                anchors.verticalCenter: contrastLeft.verticalCenter
                anchors.left: labelcontrast.left;anchors.leftMargin:140;
                from: -0.5;to:0.5;stepSize: 0.1;
                snapMode:Slider.SnapAlways
                value: piService.m_contrastLeft;
                onValueChanged: {
                    if(visible&&(sliderContrastLeft.value!=piService.contrastLeft&&
                                 !piService.setContrastLeft(sliderContrastLeft.value))){
                        showAlertPopup(qsTr("调节失败!"));
                        sliderContrastLeft.value = piService.contrastLeft;
                    }
                    if (contrastLockButton.pressStatus == 1) {
                        piService.setContrastRight(sliderContrastLeft.value)
                        sliderContrastRight.value = sliderContrastLeft.value;
                    }
                }
            }

            MySlider {
                id: sliderContrastRight
                width: 340
                anchors.verticalCenter: contrastRight.verticalCenter
                anchors.left: labelcontrast.left;anchors.leftMargin:140;
                from: -0.5;to:0.5;stepSize: 0.1;
                snapMode:Slider.SnapAlways
                value: piService.m_contrastRight;
                onValueChanged: {
                    if(visible&&(sliderContrastRight.value!=piService.displayQuality)&&
                            !piService.setContrastRight(sliderContrastRight.value)){
                        showAlertPopup(qsTr("调节失败!"));
                        sliderContrastRight.value = piService.contrastRight;
                    }
                    if (contrastLockButton.pressStatus == 1) {
                        piService.setContrastLeft(sliderContrastRight.value)
                        sliderContrastLeft.value = sliderContrastRight.value;
                    }
                }
            }

            TextButton{
                id:btnContrastRightRestore
                text: qsTr("默认值")
                visible: true
                width: btnWidthRestore
                fontSize:13
                anchors.right: parent.right
                anchors.verticalCenter: sliderContrastRight.verticalCenter
                mouseArea.onClicked:{
                        if(visible&&!piService.setContrastRight(0)){
                            showAlertPopup(qsTr("调节失败!"));
                        } else {
                            sliderContrastRight.value = 0;
                        }
                    }
            }


            TextButton{
                id:btnContrastLeftRestore
                text: qsTr("默认值")
                visible: true
                width: btnWidthRestore
                fontSize:13
                anchors.right: parent.right
                anchors.verticalCenter: sliderContrastLeft.verticalCenter
                mouseArea.onClicked:{
                        if(visible&&!piService.setContrastLeft(0)){
                            showAlertPopup(qsTr("调节失败!"));
                        } else {
                            sliderContrastLeft.value = 0;
                        }
                    }
            }

            Rectangle{
                id: contrastLeftValue;
                width: 80;
                height: labelRenderValue.height+10;
                radius: rRADIUS;
                anchors.verticalCenter: sliderContrastLeft.verticalCenter
                anchors.left: sliderContrastLeft.right; anchors.margins: 5
                color:white_pure
                MyLabel {
                    id: labelContrastLeftValue
                    anchors.centerIn: parent;
                    text: parseInt(parseInt(sliderContrastLeft.value*101)/10)
                    color:black
                }
            }

            Rectangle{
                id: contrastRightValue;
                width: 80;
                height: labelRenderValue.height+10;
                radius: rRADIUS;
                anchors.verticalCenter: sliderContrastRight.verticalCenter
                anchors.left: sliderContrastRight.right; anchors.margins: 5
                color:white_pure
                MyLabel {
                    id: labelContrastRightValue
                    anchors.centerIn: parent;
                    text: parseInt(parseInt(sliderContrastRight.value*101)/10)
                    color:black
                }
            }
        }

        //亮度
        Rectangle{
            id:brightness;
            anchors.top: contrast.bottom;anchors.topMargin: visible?30:0
            anchors.left: displayLabel.left
            anchors.right:displayLine.right
            //visible: piService.displayQualityCtrl&&piService.modeType==0
            height: visible?100:0

            MyLabel {
                id: labelbrightness
                text: qsTr("亮度")
            }
            ImageButton2 {
                id: brightnessLockButton
                image_normal: "qrc:/resource/lock2.png"
                image_pressed: "qrc:/resource/lock1.png"
                anchors.top: labelbrightness.bottom;anchors.topMargin: 19
            }
            MyLabel {
                id: brightnesstLeft
                text: qsTr("左")
                anchors.top: labelbrightness.bottom; anchors.topMargin: 10
                anchors.left: parent.left; anchors.leftMargin: 45
            }
            MyLabel {
                id: brightnessRight
                text: qsTr("右")
                anchors.top: brightnesstLeft.bottom; anchors.topMargin: 15
                anchors.left: parent.left; anchors.leftMargin: 45
            }

            MySlider {
                id: sliderBrightnessLeft
                width: 340
                anchors.verticalCenter: brightnesstLeft.verticalCenter
                anchors.left: labelbrightness.left;anchors.leftMargin:140;
                from: -0.1;to:0.1;stepSize: 0.02;
                snapMode:Slider.SnapAlways
                value: piService.m_brightnessLeft;
                onValueChanged: {
                    if(visible&&(sliderBrightnessLeft.value!=piService.brightnessLeft)&&
                            !piService.setBrightnessLeft(sliderBrightnessLeft.value)){
                        showAlertPopup(qsTr("调节失败!"));
                        sliderBrightnessLeft.value = piService.brightnessLeft;
                    }
                    if(brightnessLockButton.pressStatus == 1) {
                        piService.setBrightnessRight(sliderBrightnessLeft.value)
                        sliderBrightnessRight.value = sliderBrightnessLeft.value;
                    }
                }
            }

            MySlider {
                id: sliderBrightnessRight
                width: 340
                anchors.verticalCenter: brightnessRight.verticalCenter
                anchors.left: labelbrightness.left;anchors.leftMargin:140;
                from: -0.1;to:0.1;stepSize: 0.02;
                snapMode:Slider.SnapAlways
                value: piService.m_brightnessRight;
                onValueChanged: {
                    if(visible&&(sliderBrightnessRight.value!=piService.brightnessRight)&&
                            !piService.setBrightnessRight(sliderBrightnessRight.value)){
                        showAlertPopup(qsTr("调节失败!"));
                        sliderBrightnessRight.value = piService.brightnessRight;
                    }
                    if(brightnessLockButton.pressStatus == 1) {
                        piService.setBrightnessLeft(sliderBrightnessRight.value)
                        sliderBrightnessLeft.value = sliderBrightnessRight.value;
                    }
                }
            }

            TextButton{
                id:btnBrightnessRestore
                text: qsTr("默认值")
                visible: true
                width: btnWidthRestore
                fontSize:13
                anchors.right: parent.right
                anchors.verticalCenter: sliderBrightnessRight.verticalCenter
                mouseArea.onClicked:{
                        if(visible&&!piService.setBrightnessRight(0)){
                        showAlertPopup(qsTr("调节失败!"));
                        } else {
                            sliderBrightnessRight.value = 0;
                        }
                    }
            }


            TextButton{
                id:btnBrightnessLeftRestore
                text: qsTr("默认值")
                visible: true
                width: btnWidthRestore
                fontSize:13
                anchors.right: parent.right
                anchors.verticalCenter: sliderBrightnessLeft.verticalCenter
                mouseArea.onClicked:{
                        if(visible&&!piService.setBrightnessLeft(0)){
                        showAlertPopup(qsTr("调节失败!"));
                        } else {
                            sliderBrightnessLeft.value = 0;
                        }
                    }
            }

            Rectangle{
                id: brightnessLeftValue;
                width: 80;
                height: labelRenderValue.height+10;
                radius: rRADIUS;
                anchors.verticalCenter: sliderBrightnessLeft.verticalCenter
                anchors.left: sliderBrightnessLeft.right; anchors.margins: 5
                color:white_pure
                MyLabel {
                    id: labelBrightnessLeftValue
                    anchors.centerIn: parent;
                    text: parseInt(parseInt(sliderBrightnessLeft.value*1001)/20)
                    color:black
                }
            }

            Rectangle{
                id: brightnessRightValue;
                width: 80;
                height: labelRenderValue.height+10;
                radius: rRADIUS;
                anchors.verticalCenter: sliderBrightnessRight.verticalCenter
                anchors.left: sliderBrightnessRight.right; anchors.margins: 5
                color:white_pure
                MyLabel {
                    id: labelBrightRightValue
                    anchors.centerIn: parent;
                    text: parseInt(parseInt(sliderBrightnessRight.value*1001)/20)
                    color:black
                }
            }
        }



        //瞳距偏移值
        Rectangle{
            id:ipdRender;
            anchors.top: brightness.bottom;anchors.topMargin: visible?30:0
            anchors.left: displayLabel.left
            anchors.right:displayLine.right
            visible: (piService.hmd_name.indexOf("5K Plus")>0||piService.hmd_name.indexOf("8K")>0||
                      piService.hmd_name.indexOf("5K XR")>0)
            height: visible?30:0

            MyLabel {
                id: ipdLabelRender
                text: qsTr("瞳距偏移值")
            }
            MySlider {
                id: ipdSliderRender
                width: 340
                anchors.verticalCenter: ipdLabelRender.verticalCenters
                anchors.left: ipdLabelRender.left;anchors.leftMargin:140;
                from: -10;to:10;stepSize: 0.5;
                snapMode:Slider.SnapAlways;
                value: piService.m_lensSeparation*1000;
                onValueChanged: {
                    if(visible&&!piService.setLenSeparation(ipdSliderRender.value/1000)){
                        showAlertPopup(qsTr("调节失败!"));
                        ipdSliderRender.value = piService.m_lensSeparation;
                    }
                }
            }

            TextButton{
                id:btnIPDRestore
                text: qsTr("默认值")
                width: btnWidthRestore
                fontSize:13
                anchors.right: parent.right
                anchors.verticalCenter: ipdSliderRender.verticalCenter
                mouseArea.onClicked:{
                            if(visible&&!piService.setLenSeparation(0)) {
                                showAlertPopup(qsTr("调节失败!"))
                            } else {
                                ipdSliderRender.value = 0;
                            }
                    }
            }

            Rectangle{
                id: ipdRectRenderValue;
                width: 80;
                height: ipdLabelRenderValue.height+10;
                radius: rRADIUS;
                anchors.verticalCenter: ipdLabelRender.verticalCenter
                anchors.left: ipdSliderRender.right; anchors.leftMargin:5
                color:white_pure
                MyLabel {
                    id: ipdLabelRenderValue
                    anchors.centerIn: parent;
                    //text: parseInt(ipdSliderRender.value*100)/100;
                    text:parseInt(ipdSliderRender.valueAt(ipdSliderRender.position).toFixed(1)*10)/10;
                    color:black
                }
            }
        }

        //瞳距调节
        Rectangle{
            id:ipdRender1;
            anchors.top: ipdRender.bottom;anchors.topMargin: visible?30:0
            anchors.left: displayLabel.left
            anchors.right:displayLine.right
            visible: piService.ipdCtrl&&piService.modeType==0
            height: visible?30:0

            MyLabel {
                id: ipdLabelRender1
                text: qsTr("瞳距调节")
            }
            MySlider {
                id: ipdSliderRender1
                width: 340
                anchors.verticalCenter: ipdLabelRender1.verticalCenters
                anchors.left: ipdLabelRender1.left;anchors.leftMargin:140;
                 from: 50;to:100;stepSize: 0.1;
                snapMode:Slider.SnapAlways;
                value: piService.ipd;
                onValueChanged: {
                    if(visible&&(ipdSliderRender1.value!==piService.ipd)&&
                           !piService.writeIpdValue(ipdSliderRender1.value)){
                        showAlertPopup(qsTr("瞳距调节失败!"));
                        ipdSliderRender1.value = piService.ipd;
                    }
                }
            }

            TextButton{
                id:btnIPDRestore1
                text: qsTr("默认值")
                width: btnWidthRestore
                fontSize:13
                anchors.right: parent.right
                anchors.verticalCenter: ipdSliderRender1.verticalCenter
                mouseArea.onClicked:{
                            if(visible&&!piService.SetIpdDefault())
                             showAlertPopup(qsTr("瞳距调节失败!"));
                    }
            }

            Rectangle{
                id: ipdRectRenderValue1;
                width: 80;
                height: ipdLabelRenderValue1.height+10;
                radius: rRADIUS;
                anchors.verticalCenter: ipdLabelRender1.verticalCenter
                anchors.left: ipdSliderRender1.right; anchors.leftMargin:5
                color:white_pure
                MyLabel {
                    id: ipdLabelRenderValue1
                    anchors.centerIn: parent;
                    //text: parseInt(ipdSliderRender1.value*100)/100;
                    text:parseInt(ipdSliderRender1.valueAt(ipdSliderRender1.position).toFixed(1)*10)/10;
                    color:black
                }
            }
        }


        MyLabel{
            id:calibrationLabel
            visible: btnCalibrate.visible || btnHorizon.visible
            text:qsTr("校准")
            anchors.top:ipdRender1.bottom;anchors.topMargin: visible?20:0
        }
        Rectangle{//line
            id:calibrationLine;
            width: parent.width;height: 1;
            visible: calibrationLabel.visible
            color: white;
            anchors.left: calibrationLabel.right;anchors.leftMargin: 5
            anchors.right: parent.right;anchors.rightMargin: 20
            anchors.verticalCenter: calibrationLabel.verticalCenter
        }
        //磁校准
        TextButton {
            id: btnCalibrate
            width:btnWidth;height: visible?txt.height+10:0
            anchors.top: calibrationLabel.bottom;anchors.topMargin: visible?10:0
            anchors.left: calibrationLabel.left
            text: qsTr("磁校准")
            fontSize:13
            mouseArea.onClicked: {
                showCalibrateDialogNew();
            }
            visible: piService.connect&&piService.modeType==0&&piService.hmd_name.indexOf("8K")<=0
        }
        //水平校准
        TextButton {
            id: btnHorizon
            width:btnWidth;height: visible?txt.height+10:0
            anchors.top: btnCalibrate.bottom;anchors.topMargin: visible?10:0
            anchors.left: calibrationLabel.left
            text: qsTr("水平校准")
            fontSize:13
            mouseArea.onClicked: {
                if(piService.slopeCalibration()){
                    showAlertPopup2(qsTr("水平校准成功!"));
                }else{
                    showAlertPopup2(qsTr("水平校准失败!"));
                }

            }
            visible: piService.connect&&piService.modeType==0&&!piService.tracker1&&!piService.tracker2&&!piService.lighthouseEnabled
        }
        Loader{
            id:dialogCalibrateNew
        }

        MyLabel{
            id:otherLabel
            text:qsTr("其他")
            anchors.top:btnHorizon.bottom;anchors.topMargin: 20
        }
        Rectangle{//line
            id:otherLine;
            width: parent.width;height: 1;
            color: white;
            anchors.left: otherLabel.right;anchors.leftMargin: 5
            anchors.right: parent.right;anchors.rightMargin: 20
            anchors.verticalCenter: otherLabel.verticalCenter
        }
        TextButton{
            id:resetHmd
            width:btnWidth;height: visible?txt.height+10:0
            anchors.top: otherLabel.bottom;anchors.topMargin: visible?10:0
            anchors.left: otherLabel.left
            text: qsTr("重启头盔")
            visible: piService.connect
            mouseArea.onClicked: {
                showMessageDialog2(qsTr("确认"),qsTr("取消"),qsTr("是否确认重启头盔？"),function(){
                    piService.rebootHmd();
                },function(){
                    return;
                }
                );
            }
        }
        TextButton {
            id:reService
            width:btnWidth;height: visible?txt.height+10:0
            anchors.top: resetHmd.bottom;anchors.topMargin: visible?10:0
            text: qsTr("重启服务")
            visible: piService.connect
            mouseArea.onClicked: {
                showMessageDialog2(qsTr("确认"),qsTr("取消"),qsTr("是否确认重启服务？"),function(){
                    resourceModel.reService();
                },function(){
                    return;
                }
                );
            }
        }

        TextButton {
            id: btnRestore
            anchors.top: resetHmd.bottom;anchors.topMargin: 60
            anchors.right: parent.right;anchors.rightMargin: 20
            text: qsTr("恢复默认")
            fontSize: 14;
            size:large
            visible: true;
            mouseArea.onClicked: {
                piService.resetFactory();
                if(lighthouseBox.visible){
                    piService.setLighthouseEnabled(true);
                    lighthouseBox.checked = true;
                }
                if (piService.displayHmdName == "Pimax 8K" &&
                        piService.displayHmdName == "Pimax 5K Plus" &&
                        piService.displayHmdName =="Pimax 5K") {
                    piService.setFovType(1);
                }
                piService.setCompatibleSteamvrUsingParallel(false);
                piService.EnablehiddenArea(true);
                hiddenArea.checked = true;
                compatibleBox.checked = false;
                piService.writeEnableGyro(false);
                checkBox1.checked = false;
                piService.SetIpdDefault();
                piService.SetRenderDefault();
                piService.SetBrightDefault();
                labelRefresh?piService.setRefreshByIndex(0):false;

                piService.setContrastLeft(0);
                sliderContrastLeft.value = 0;
                piService.setContrastRight(0);
                sliderContrastRight.value = 0;
                piService.setBrightnessLeft(0);
                sliderBrightnessLeft.value = 0;
                piService.setBrightnessRight(0);
                sliderBrightnessRight.value = 0;
                piService.setLenSeparation(0);
                ipdSliderRender.value = 0;
                contrastLockButton.pressStatus = 0;
                brightnessLockButton.pressStatus = 0;
            }
        }
    }
    function getResolutionIndex(resolution){
        var count = listmodel.columnCount();
        for(var i = 0; i < count; i++){
            if(listmodel.get(i).cost == resolution){
                return i;
            }
        }
    }

    function showSwitchConfirmDialog(type,checked){
        showMessageDialog(qsTr("更改此设置,头盔将自动断开重新连接,<br/>是否确认修改?"),
            function(){
                if(type==1){
                    if(!piService.writeEnableGyro(checked)){
                        showAlertPopup(qsTr("陀螺仪设置失败!"));
                        checkBox1.checked = !checked;
                    }
                }else if(type==2){
                    if(!piService.writeCoolTone(checked)){
                        showAlertPopup(qsTr("冷色调设置失败!"));
                        checkBox2.checked = !checked;
                    }
                }else if(type==4){
                if(!piService.writeResolution(checked)){
                    showAlertPopup(qsTr("屏幕分辨率设置失败!"));
                }
            }

            },
            function(){
                if(type==1){
                    checkBox1.checked = !checked;
                }else if(type==2){
                    checkBox2.checked = !checked;
                }else if(type==4){
                    comboSolution.currentIndex = piService.resolutionIndex;
                }
            });
    }
    function showCalibrateDialogNew(){
        showMessageDialog(qsTr("磁校准将暂时无法使用头盔,请先退出游戏等！<br/>开始磁校准？"),function(){
            piService.startMagnetCalibrate();
            showDialog(dialogCalibrateNew,"SettingMagnetCalibrate.qml");
        });
    }

}
