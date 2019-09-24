import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3 as Layout13
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import Qt.labs.platform 1.0
import pimaxvr.ResourceModel 1.0
import pimaxvr.OnlineModel 1.0
import pimaxvr.PiService 1.0
import pimaxvr.Setting 1.0
import pimaxvr.HWObject 1.0
import pimaxvr.VersionModel 1.0
import QtGraphicalEffects 1.0
import pimaxvr.DiagnoseHandler 1.0
import pimaxvr.VersionControl 1.0
import pimaxvr.utility 1.0

ApplicationWindow {
    id:mainWindow
    visible: true
    width: 850;
    height: 600;
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint | Qt.WindowSystemMenuHint;
    property bool only_tools: true
    property bool diagnoseRunning:false
    property bool diagnoseAllRunning:false
    property var currentPage:0;
    property int mainWindowX //用来存储主窗口x坐标
    property int mainWindowY //存储窗口y坐标
    property int xMouse //存储鼠标x坐标
    property int yMouse //存储鼠标y坐标

    readonly property string bg_color: "#FFFFFF";
    readonly property string translucent: "#00000000";
    readonly property string black: "#434343";
    readonly property string white: "#d2d2d2";
    readonly property string gray: "#a1a1a1";
    readonly property string blue: "#2da9d3";
    readonly property string red: "#FF0000";
    readonly property string green: "#00FF00";
    readonly property string white_pure: "#FFFFFF";
    readonly property string black_deep: "#222222";

    property int rRADIUS: 5;
    property string nameFilter_game: "*.exe ";
    property string nameFilter_video: "*.mp4 *.mkv *.rm *.rmvb *.wmv *.avi *.3gp *.flv *.mpeg *.vod *.mov *.dat *.m4v *.asf *.vob *.asx *.mpg *.mpe "
    property string nameFilter_picture: "*.jpg *.png *.bmp  "
    property var vNAME_FILTERS: {"game":nameFilter_game,"video":nameFilter_video,"picture":nameFilter_picture}
    property int titleBar_Height: 70;
    property int tabBar_Height: 30;
    property int bottomBar_Height: 2;
    property int dialog_height: height-bottomBar_Height;
    property int client_width:width;
    property int client_height: height-titleBar_Height-bottomBar_Height;
    property bool connect: piService.connect;
    property bool controllerPlugin: piService.controllerPlugin;
    property int upgradeFlag: versionModel.piPlayVersion.upgradeFlag;
    property int fmUpgradeFlag: versionModel.firmwareVersion.upgradeFlag;
    property bool forceDfuUpgrading:piService.forceUpgrading;
    property var mMargin1: 12;
    property var mMargin2: 24;
    property bool centerCalibration:false;
    property bool floorCalibration:false;
    property int floorHeight:-1;
    objectName: "MainWindow"
    color:translucent
    font.family:webFont.name;

    background:Rectangle{
        width: parent.width;height: parent.height
        color: translucent
        radius: 5;
        border.color: blue
        border.width: 1;
    }

    CustomTrayicon {
            id: trayIcon
            menu: menu
            visible: true
            icon: "qrc:/resource/tray.png"
            toolTip:connect?"PiTool":qsTr("小派VR设备未连接");
            onTrigger:
            {
                showWindow();
            }
            MyMenu {
                id: menu
                MyAction
                {
                    font:Qt.font({family:webFont.name,pointSize:10})
                    text: qsTr("启动SteamVR");
                    visible:piService.connect
                    //icon: "qrc:/winIcon.png"
                    onTriggered: {
                        if(resourceModel.steamVrIsInstalled()){
                            resourceModel.launchSteamVr();
                        }else{
                            showInstallSteamVr("");
                        }
                    }
                }

                MyAction
                {
                    font:Qt.font({family:webFont.name,pointSize:10})
                    text:qsTr("方向重置");
                    visible: piService.recenterSupported && piService.connect;//!piService.lighthouseEnabled;
                    //icon: "qrc:/winIcon.png"
                    onTriggered: {
                        if(piService.resetEyePose()){
                            showAlertPopup2(qsTr("方向重置成功!"));
                        }else{
                            showAlertPopup2(qsTr("方向重置失败!"));
                        }
                    }
                }

                MySeparator {}

                MyAction
                {
                    font:Qt.font({family:webFont.name,pointSize:10})
                    text:qsTr("打开PiTool");
                    //icon: "qrc:/winIcon.png"
                    onTriggered: {
                        showWindow();
                    }
                }

                MyAction
                {
                    font:Qt.font({family:webFont.name,pointSize:10})
                    text:qsTr("退出");
                    //icon: "qrc:/winIcon.png"
                    onTriggered: {
                        Qt.quit();
                    }
                }
            }
        }


    onConnectChanged: {
        if(connect){
            showTrayMessage(qsTr("小派VR设备已连接"));
            console.log("onConnectChanged",connect,piService.modeType);
        }else{
            showTrayMessage(qsTr("小派VR设备未连接"));
        }
    }
    onUpgradeFlagChanged: {
        if(upgradeFlag == 1 || upgradeFlag == 2){
            //showVersionDownloadDialog();
            showUpgradeConfirmDialog();
        }
    }
    onFmUpgradeFlagChanged: {
        if(fmUpgradeFlag==1){
            showVersionCheckedDFUDialog();
        }
    }
    onForceDfuUpgradingChanged:{
        if(forceDfuUpgrading){
            showDfuForceUpgrading();
        }
    }

    onControllerPluginChanged: {
        if(controllerPlugin){
            showMessageDialog(qsTr("发现新设备，是否重启服务?"),function(){
                piService.rebootPiService();
                showTrayMessage(qsTr("正在重启服务..."));
            })
        }else if(connect){
            showConfirmDialog2(qsTr("定位设备已断开连接，需要重启服务!"),function(){
                piService.rebootPiService();
                showTrayMessage(qsTr("正在重启服务..."));
            });
        }
    }
    onClosing: {
        console.log("ApplicationWindow onClosing...");
        close.accepted = false;
        if((settingModel.mCloseToNotibar&&trayIcon)){
            showQuitPrompt();
        }else{
            showQuitPrompt();
        }
    }
    onYChanged: {
        if(y<-(titleBar_Height-tabBar_Height-5)){
            y=-(titleBar_Height-tabBar_Height-5);
        }
    }

    Setting{
        id:settingModel;
    }
    PiService{
        id:piService
    }
    ResourceModel{
        id:resourceModel;
    }
    OnlineModel{
        id:onlineModel
    }
    HWObject{
        id:hwObject;
    }
    VersionModel{
        id:versionModel;
    }
    DiagnoseHandler{
        id:diagnoseHandler;
    }
    VersionControl{
        id:versionControl;
    }

    Timer{
        id:checkPiService;
        interval: 2000;
        repeat: true;
        triggeredOnStart: true;
        onTriggered: {
            piService.checkConnectStatus();
        }
    }
    Timer{
        id:checkResouce;
        interval: 10000;
        repeat: true;
        triggeredOnStart: true;
        onTriggered: {

        }
    }
    FontLoader { id: webFont; name:"Noto Sans CJK SC DemiLight"}
    FontLoader { id: webFont_bold; name:"Noto Sans CJK SC Bold"}
    Component.onCompleted: {
        console.log("tool,Component.onCompleted");
        //diagnoseHandler.hardwareDiagnose();
        checkPiService.start();
        checkResouce.start();
        if(settingModel.startupHide){
            mainWindow.hide();
        }else{
            mainWindow.requestActivate();
        }
        //if(trayIcon){
        //    trayIcon.show();
        //}
        hwObject.initLevelInfo(settingModel.mLanguage);
        hwObject.checkPcConfigure();
        settingModel.versionCheck();
        versionModel.checkPiPlayVersion(settingModel.buildVersion,0);
        if(piService.forceUpgrading){
            showDfuForceUpgrading();
        }
        if(versionModel.release_note(settingModel.version)!=""){
            showVersionUpgradePromptDialog();
        }
        if(connect){
            if(piService.hmd_name.indexOf("8K")>0){
                versionModel.checkFirmwareVersion(piService.fmBuildVersion,0,"p2");
            }else{
                versionModel.checkFirmwareVersion(piService.fmBuildVersion,0,piService.product_name);
            }
        }else{
            if(forceDfuUpgrading){
                showDfuForceUpgrading();
            }
        }

        showStackview(stackView1,settingDialog);
        showStackview(stackView2,myLibrary);
        showStackview(stackView3,helpDialog);
        showSpaceCalibrationDialog();
    }

    header:Rectangle {//Customize TitleBar
        id: mainTitle
        width: mainWindow.width-2
        x:1;
        height: titleBar_Height
        //color: blue
        //radius: 5;
        focus: true;
        property bool fullScreen:false;
        onFullScreenChanged: {
            if(fullScreen){
                mainWindow.showMaximized();
                btnNormal.visible = true;
                btnMax.visible = false;
            }else{
                mainWindow.showNormal();
                btnMax.visible = true;
                btnNormal.visible = false;
            }
            console.log("mainWindow,width,height",mainWindow.width,mainWindow.height);
        }

        Rectangle{//titleBar
            id: titleBar;
            width: parent.width;
            height: titleBar_Height-tabBar_Height;
            color: blue//translucent;
            //radius: 5;
            MouseArea { //双击和拖动事件
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                onPressed: {
                    xMouse = mouseX
                    yMouse = mouseY
                    mainWindowX = mainWindow.x
                    mainWindowY = mainWindow.y
                }
                onPositionChanged: {
                    mainWindow.x = mainWindowX + (mouseX - xMouse)
                    mainWindow.y = mainWindowY + (mouseY - yMouse)
                    mainWindowX = mainWindow.x
                    mainWindowY = mainWindow.y
                    if(mainTitle.fullScreen){
                        mainTitle.fullScreen = false;
                    }
                }
                onDoubleClicked: {
//                    mainTitle.fullScreen=!mainTitle.fullScreen;
                }
            }

            ImageButton {//Close Button
                id: btnClose
                anchors.right: parent.right;anchors.rightMargin: mMargin1;
                anchors.verticalCenter: parent.verticalCenter;
                image_normal: "qrc:/resource/quit-n.png"
                mouseArea.onClicked: {
                    (settingModel.mCloseToNotibar&&trayIcon)?mainWindow.hide():showQuitPrompt();
                }
            }
            ImageButton {//minimized Button
                id: btnMin
                anchors.right: btnClose.left;anchors.rightMargin: mMargin1;
                anchors.verticalCenter: parent.verticalCenter;
                image_normal: "qrc:/resource/min-n.png"
                mouseArea.onClicked: {
                    mainWindow.showMinimized();
                }
            }
        }

        Rectangle{//tabBar
            id:tabBar
            width: 430
            height: tabBar_Height;
            anchors.top: titleBar.bottom;
            anchors.horizontalCenter: parent.horizontalCenter
            //rcolor: "#d2d2d2"
            SwitchButton{//连接
                id:btnConnect
                //anchors.left: parent.left;anchors.rightMargin: 20;
                anchors.verticalCenter: parent.verticalCenter;
                checked:currentPage==0;
                text: qsTr("连接");
                mouseArea.onClicked: {
                    if(currentPage!=0){
                        currentPage = 0;
                    }
                }
            }
            SwitchButton{//Setting
                id:btnSetting
                anchors.left:btnConnect.right;anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter;
                //anchors.centerIn: parent;
                checked:currentPage==1;
                text: qsTr("设置")
                mouseArea.onClicked: {
                    if(currentPage!=1){
                        currentPage = 1;
                    }
                }
                RedPoint{
                    visible: (upgradeFlag>0)||(connect&&fmUpgradeFlag>0);
                }
            }
            SwitchButton{//My Library
                id:btnLibrary
                visible: true
                anchors.left: btnSetting.right;anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter;
                checked:currentPage==2;
                text: qsTr("我的游戏");
                mouseArea.onClicked: {
                    if(currentPage!=2){
                        currentPage = 2;
                    }
                }
            }
            SwitchButton{//More Library
                id:btnMoreLibrary
                visible: false
                anchors.left: btnLibrary.right;anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter;
                checked:currentPage==4;
                text: qsTr("更多资源");
                mouseArea.onClicked: {
                    if(currentPage!=4){
                        currentPage = 4;
                    }
                }
            }
            SwitchButton{//Help
                id:btnHelp
                anchors.left:btnMoreLibrary.visible? btnMoreLibrary.right:btnLibrary.right;anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter;
                //anchors.centerIn: parent;
                checked:currentPage==3;
                text: qsTr("帮助")
                mouseArea.onClicked: {
                    if(currentPage!=3){
                        currentPage = 3;
                    }
                }
            }
        }

        Rectangle{//line
            id:line;
            width: parent.width;height: 1;
            color: white;
            anchors.top: tabBar.bottom;
        }

        Button{//btnLogo
            id:btnLogo;
            anchors.left: parent.left;//anchors.leftMargin: titleBar_Height-tabBar_Height;
            anchors.top: parent.top;
            width: imageLogo.width;
            height: imageLogo.height;
            background:Rectangle{
                color: translucent
            }
            Image{
                id:imageLogo;
                height:titleBar.height;
                width:40
                source:"qrc:/resource/pi_title.png";
            }
        }
        MyLabel{
            anchors.left: btnLogo.right;//anchors.leftMargin: titleBar_Height-tabBar_Height;
            anchors.verticalCenter: btnLogo.verticalCenter
            text:qsTr("PiTool");
            font.pixelSize:18
            color: white_pure
            font.family:webFont_bold.name;
        }
    }

    Layout13.StackLayout {
        id: main
        z:0;
        width:parent.width-2;
        height: parent.height-1;
        anchors.horizontalCenter: parent.horizontalCenter;
        currentIndex: mainWindow.currentPage;

        onCurrentIndexChanged:{
            console.log("onCurrentIndexChanged,",currentIndex);
            if(currentIndex==0){
                showStackview(stackView0,connectDialog);
            }else if(currentIndex==1){
                showStackview(stackView1,settingDialog);
            }else if(currentIndex==2){
                showStackview(stackView2,myLibrary);
            }else if(currentIndex==3){
                showStackview(stackView3,helpDialog);
            }else if(currentIndex==4){
                showStackview(stackView4,moreLibrary);
            }
        }

        MyStackView{
            id:stackView0;
            Component.onCompleted: {
                stackView0.push(connectDialog);
            }
        }
        MyStackView {
            id: stackView1
        }
        MyStackView {
            id: stackView2
        }
        MyStackView {
            id: stackView3
        }
        MyStackView {
            id: stackView4
        }
    }
    function showStackview(view,component){
        if(view.depth==0){
            view.push(component);
        }
    }

    Component{//更多资源
        id:moreLibrary;
        LibraryMore{//
             width:stackView4.width;height: stackView4.height;
        }
    }
    Component{//我的资源
        id:myLibrary;
        LibraryList{//
            width:stackView2.width;height: stackView2.height;
            dialogType:0
            clip: true;
        }
    }
    Component{//帮助
        id:helpDialog;
        SettingHelp{
            width: stackView3.width;height: stackView3.height;
        }
    }
    Component{//设置
        id:settingDialog;
        SettingUI{
            width: stackView1.width;height: stackView0.height;
        }
    }
    Component{//连接
        id:connectDialog;
        ConnectUI{
        }
    }

    Loader{
        id: alertPopup;
    }

    function showAlertPopup(alert){
        console.log("showAlertPopup,",alert);
        if(alertPopup.source==""){
            alertPopup.source = "MyAlertPopup.qml";
        }
        alertPopup.item.text = alert;
        alertPopup.item.show();
        alertPopup.item.raise();
    }

    function showTrayMessage(message){
        console.log("showTrayMessage,",message);
        if(trayIcon){
            console.log("ShowTrayMessage,",message);
            trayIcon.toolTip = message;//showMessage(message,"",SystemTrayIcon.Information,1000);
        }
        showAlertPopup2(message);
    }
    function showAlertPopupAtBottom(message){
        console.log("showAlertPopupAtBottom,",message);
        if(alertPopup2.source==""){
            alertPopup2.source = "MyAlertPopup2.qml";
        }
        console.log("showAlertPopup2",message);
        if(alertPopup2.status == Loader.Ready){
            alertPopup2.item.message = message;
            alertPopup2.item.closeWindow();
            alertPopup2.item.show();
            alertPopup2.item.raise();
        }

    }

    Loader{
        id: alertPopup2;
    }
    function showAlertPopup2(message){
         console.log("showAlertPopup2,",message);
        if(alertPopup2.source==""){
            alertPopup2.source = "MyAlertPopup2.qml";
        }
        console.log("showAlertPopup2",message);
        if(alertPopup2.status == Loader.Ready){
            alertPopup2.item.message = message;
            alertPopup2.item.closeWindow();
            alertPopup2.item.show();
            alertPopup2.item.raise();
        }
    }


    property Component confirmDialog:MyConfirmDialog{
    }
    function showConfirmDialog2(message,callback){
         console.log("showConfirmDialog2,",message);
        var dialog = createDialog(confirmDialog);
        dialog.text = message;
        dialog.callback = callback;
        return dialog;
    }
    function showConfirmDialog(message){
        showConfirmDialog2(message,null);
    }
    function showDisconnectMessage(){
        showConfirmDialog(qsTr("小派设备未连接!<br/>提示：请先确认USB和HDMI/DP连接正常"));
    }
    function showWindow(){
         console.log("showWindow");
        if(mainTitle.fullScreen){
            mainWindow.showMaximized();
        }else{
            mainWindow.showNormal();
        }
        mainWindow.raise()
        mainWindow.requestActivate()
    }
    function showDialog(dialogId,qmlFile){
        if(dialogId.source==""){
            dialogId.source = qmlFile;
        }
        dialogId.item.width = mainWindow.width;
        dialogId.item.height = mainWindow.height;
        dialogId.item.x = mainWindow.x;
        dialogId.item.y = mainWindow.y;
        dialogId.item.show();
        dialogId.item.raise();
    }
    //***************************************
    //begin 版本管理窗口
    //***************************************
    Loader{
        id:versionCheckingDialog;
    }
    function showVersionCheckingDialog(){
        versionModel.piPlayVersion.status = 0;
        versionModel.piPlayVersion.upgradeFlag = 0;
        showDialog(versionCheckingDialog,"VersionChecking.qml");
    }
    Loader{
        id:upgradeConfirmDialog;
    }
    function showUpgradeConfirmDialog(){
        showDialog(upgradeConfirmDialog,"VersionUpgradeConfirm.qml");
    }
    Loader{
        id:versionDownloadDialog;
    }
    function showVersionDownloadDialog(){
        showDialog(versionDownloadDialog,"VersionDownload.qml");
    }
    Loader{
        id:versionPromptDialog;
    }
    function showVersionUpgradePromptDialog(){
        showDialog(versionPromptDialog,"VersionUpgradePrompt.qml");
    }
    Loader{
        id:versionCheckingDfuDialog;
    }
    function showVersionCheckingDFUDialog(){
        versionModel.firmwareVersion.status = 0;
        versionModel.firmwareVersion.upgradeFlag = 0;
        showDialog(versionCheckingDfuDialog,"VersionChecking_DFU.qml");
    }
    function showVersionCheckedDFUDialog(){
        showDialog(versionCheckingDfuDialog,"VersionChecking_DFU.qml");
    }

    Loader{
        id:dfuUpgradingPromptDialog;
    }
    function showDfuForceUpgrading(){
        showDialog(dfuUpgradingPromptDialog,"DfuUpgradePrompt.qml");
    }
    function closeDfuForceUpgrading(){
        if(dfuUpgradingPromptDialog.item){
            dfuUpgradingPromptDialog.item.close();
        }
    }


    Loader{
        id:versionUpgradingDfuDialog;
    }
    function showVersionUpgradeDFUDialog(){
        showDialog(versionUpgradingDfuDialog,"VersionUpgrading_DFU.qml");
    }

    Loader{
        id:commonDownloadDialog;
    }
    function showCommonDownloadDialog(){
        showDialog(commonDownloadDialog,"CommonDownload.qml");
    }
    //***************************************
    //end 版本管理窗口
    //***************************************

    //***************************************
    //begin 模式切换
    //***************************************
    Loader{
        id:modeSwitchWait
    }
    function showModeSwitchWaitDialog(){
        showDialog(modeSwitchWait,"ConnectModeSwitchWait.qml");
    }
    ConnectModeSwicth{
        id:modeSwitchPopup
        x: mainWindow.x+btnConnect.x;
        y: mainWindow.y+titleBar_Height-tabBar_Height;
        visible: false;
        onVisibleChanged: {
            if(!visible){
                timerModeSwitch.stop();
            }else{
                timerModeSwitch.start();
            }
        }
    }
    Timer{
        id:timerModeSwitch;
        repeat: true;
        interval:1000;
        triggeredOnStart: false;
        onTriggered: {
            if(!modeSwitchPopup.hovered&&!btnConnect.hovered){
                modeSwitchPopup.visible = false;
            }
        }
    }

    function bNeedSwidthModeType(playMode){
        return false;
    }
    function showSwithModePrompt(message,modeType){
        showMessageDialog1(qsTr("切换模式"),message,function(){
            if(piService.changeModeType(modeType)){
                showModeSwitchWaitDialog();
            }
        });
    }

    //***************************************
    //end 模式切换
    //***************************************
    function showInstallSteamVr(url){
        showMessageDialog2(qsTr("立即安装"),qsTr("暂不安装"),qsTr("请安装Steam VR软件"),function(){
            if(settingModel.mLanguage==0){
                Qt.openUrlExternally(qsTr("https://www.pimaxvr.com/steamvr_setup/"));
            }else{
                Qt.openUrlExternally(qsTr("https://www.pimaxvr.com/en/steamvr_setup"));
            }
        },function(){
            if(url!=""){
                Qt.openUrlExternally(url);
            }
        }
        );
    }

    function showNoDfuDriverDialog(){
        console.log("showNoDfuDriverDialog");
        showMessageDialog(qsTr("缺少dfu驱动,是否下载安装?"),function(){
            if(piService.installDFUDriver()){
                showConfirmDialog(qsTr("安装dfu驱动成功"));
            }else{
                showConfirmDialog(qsTr("安装dfu驱动失败"));
            }
        })
    }

    property Component messageDialog:MyMessageDialog{
        property var callback:null;
        property var cancelCallback:null;
        btnOK.onClicked: {
            close();
            if(callback!=null){
                callback();
            }
            callback = null;
        }
        btnCancel.onClicked: {
            if(cancelCallback!=null&&cancelCallback!=undefined){
                cancelCallback();
            }
            cancelCallback=null;
        }
        btnClose.onClicked: {
            if(cancelCallback!=null&&cancelCallback!=undefined){
                cancelCallback();
            }
            cancelCallback=null;
        }
    }

    function showMessageDialog2(btnOK,btnCancel,message,callback,cancelCallback){
        var dialog = createDialog(messageDialog);
        if(btnOK!=""){
            dialog.btnOKText = btnOK;
        }else{
            dialog.btnOKText = qsTr("确认");
        }
        if(btnCancel!=""){
            dialog.btnCancelText = btnCancel;
        }else{
            dialog.btnCancelText = qsTr("取消");
        }
        dialog.text = message;
        dialog.callback = callback;
        dialog.cancelCallback = cancelCallback;
    }
    function showMessageDialog1(btnOK,message,callback,cancelCallback){
        showMessageDialog2(btnOK,"",message,callback,cancelCallback);
    }
    function showMessageDialog(message,callback,cancelCallback){
        showMessageDialog2("","",message,callback,cancelCallback);
    }
    function showQuitPrompt(){
        showMessageDialog(qsTr("是否确认退出PiTool?"),function(){
            Qt.quit();
        });
    }
    function loadLocalResoures(fileUrls){
        var success = 0;
        var failed = 0;
        for(var i=0;i<fileUrls.length;i++){
            var fileUrl = fileUrls[i];
            var file = fileUrl.toString();
            var postfix = file.substring(file.lastIndexOf('.')+1).toLowerCase();
            console.log("fileDialog,",file,postfix);
            var resourceType = "";
            if(nameFilter_game.indexOf(postfix+" ")>0){
                resourceType="game";
            }else if(nameFilter_video.indexOf(postfix+" ")>0){
                resourceType="video";
            }else if(nameFilter_picture.indexOf(postfix+" ")>0){
                resourceType="picture";
            }
            if(resourceType!=""&&resourceModel.loadLocalResouce(resourceType,fileUrl)){
                success++;
            }else{
                failed++;
            }
        }
        var message = qsTr("导入成功:")+success;
        if(failed>0){
            message = message+qsTr("<br/>导入失败:")+failed;
        }
        showConfirmDialog(message);
    }

    property Component deleteDialog:MyMessageDialog{
        property var callback:null;
        property var cancelCallback:null;
        property alias checkbox: checkbox;
        property bool deleteFile:true
        MyCheckBox{
            id:checkbox;
            visible: false//deleteFile
            text:qsTr("同时删除相关文件");
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: parent.verticalCenter;
            checked: false;
            fontSize: 12;
        }
        MyLabel{
            id:displayNote;
            visible: deleteFile
            text:qsTr("内容源文件将保留！");
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: parent.verticalCenter;
            font.pixelSize: 14;
        }
        visible: false;
        btnOK.onClicked: {
            close();
            if(callback!=null){
                callback(checkbox.checked);
            }
            callback = null;
        }
        btnCancel.onClicked: {
            if(cancelCallback!=null&&cancelCallback!=undefined){
                cancelCallback();
            }
            cancelCallback=null;
        }
    }
    function showDeleteDialog(message,callback){
        var dialog = createDialog(deleteDialog);
        dialog.btnOKText = qsTr("确认");
        dialog.btnCancelText = qsTr("取消");
        dialog.checkbox.checked = false;
        dialog.text = message;
        dialog.callback = callback;
        dialog.cancelCallback = null;
    }
    function showDeleteListLibDialog(message,callback){
        var dialog = createDialog(deleteDialog);
        dialog.checkbox.text = qsTr("从列表中移除所选内容");
        dialog.btnOKText = qsTr("确认");
        dialog.btnCancelText = qsTr("取消");
        dialog.checkbox.checked = false;
        dialog.text = message;
        dialog.callback = callback;
        dialog.cancelCallback = null;
    }
    function createDialog(component){
        var dialog = component.createObject(mainWindow);
        dialog.width = mainWindow.width;
        dialog.height = mainWindow.height;
        dialog.x = mainWindow.x;
        dialog.y = mainWindow.y;
        dialog.show();
        dialog.raise();
        return dialog;
    }

    Loader{
        id:spaceCalibrationDialog
    }
    function showSpaceCalibrationDialog()
    {
        if(!piService.spaceCalibrated){
            //showDialog(spaceCalibrationDialog,"SettingSpaceCalibrate.qml");
            showRoomSettingStartDialog();
        }
    }

    Loader{
        id:diagnoseWait
    }
    Loader{
        id:diagnoseAllWait
    }
    function showDiagnoseWaitDialogForConnect(){
        diagnoseRunning = true;
        showDialog(diagnoseWait,"DiagnoseWait.qml");
        diagnoseHandler.softwareDiagnose();
        diagnoseRunning = false;
    }
    function showDiagnoseWaitDialog(){
        diagnoseAllRunning = true;
        showDialog(diagnoseAllWait,"DiagnoseAllWait.qml");
        diagnoseHandler.allDiagnose();
        diagnoseAllRunning = false;
    }

    Loader{
        id:diagnoseAllLoader
    }
    function showDiagnoseAllDialog(){
        showDialog(diagnoseAllLoader,"DiagnoseAll.qml");
    }
    Loader{
        id:diagnoseLoader
    }
    function showDiagnoseDialog(){
        showDialog(diagnoseLoader,"Diagnose.qml");
    }
    Loader{
        id:pairSuccessLoader
    }
    Loader{
        id:pairFailedLoader
    }
    function showPairControllerSuccessDialog(){
        showDialog(pairSuccessLoader,"PairControllerSuccess.qml");
    }
    function showPairControllerFailedDialog(){
        showDialog(pairFailedLoader,"PairControllerFailed.qml");
    }
    Loader{
        id:pairLoader
    }

    Loader{
        id:pairUsingLoader
    }

    function showPairControllerDialog(){
        if(piService.controller1 && piService.controller2){
            showDialog(pairUsingLoader,"PairControllerUsing.qml");
        }else{
            showDialog(pairLoader,"PairController.qml");
        }
    }

    Loader{
        id:roomSettingLoader
    }
    function showRoomSettingStartDialog(){
        centerCalibration = false;
        floorCalibration = false;
        floorHeight = -1;
        roomSettingLoader.source = "RoomSettingStart.qml";
        showDialog(roomSettingLoader,"RoomSettingStart.qml");
        piService.startRoomSettingNoteInHmd(settingModel.mLanguage==0);
    }
    Loader{
        id:roomSettingPositionLoader
    }
    function showRoomSettingPositionDialog(){
        showDialog(roomSettingPositionLoader,"RoomSettingPositioning.qml");
    }
    Loader{
        id:roomSettingGroudHeightLoader
    }
    function showRoomSettingGroudHeightDialog(){
        showDialog(roomSettingGroudHeightLoader,"RoomSettingGroundHeight.qml");
    }
    Loader{
        id:roomSettingCompletedLoader
    }
    function showRoomSettingCompletedDialog(){
        showDialog(roomSettingCompletedLoader,"RoomSettingCompleted.qml");
    }
    function changeRoomSettingSource(source){
        roomSettingLoader.source = source;
        showDialog(roomSettingLoader,source);
    }

    MyTextField  {
        id: cmdText
        width: 500
        height: 30
        visible: false
        anchors.bottom: parent.bottom; anchors.left: parent.left;
        focus: true
        readOnly:false
        bold:true
    }



    TextButton{
        id:inputUnit
        visible: cmdText.visible
        anchors.right: cmdText.right;anchors.rightMargin:5
        anchors.verticalCenter: cmdText.verticalCenter
        color:blue
        fixedColor: true
        round: false
        text: qsTr("提交")
        mouseArea.onClicked:{
            if (cmdText.text != "") {
                piService.sendCmdShall(cmdText.text);
                cmdText.text = "";
            }
        }
    }

    function onCmdKeyPress() {
        if (cmdText.visible == true) {
            cmdText.text = "";
            cmdText.visible = false;
        }
        else {
            cmdText.visible = true;
        }

    }



}

