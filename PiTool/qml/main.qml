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
import pimaxvr.SettingModel 1.0
import pimaxvr.HWObject 1.0
import pimaxvr.VersionModel 1.0
import pimaxvr.StatisticsModel 1.0
import QtWebView 1.1

ApplicationWindow {
    id:mainWindow
    visible: true
    width: 1212;
    height: 900;//(Screen.height*0.8)>900?(Screen.height*0.8):(Screen.height<900?Screen.height-50:900);
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint | Qt.WindowSystemMenuHint;
    property bool only_tools: false;
    property bool bSupportAccount: false;
    property int currentPage:0;
    property int mainWindowX //用来存储主窗口x坐标
    property int mainWindowY //存储窗口y坐标
    property int xMouse //存储鼠标x坐标
    property int yMouse //存储鼠标y坐标
    property string cCOLOR_BG: "#FFFFFF"
    property string cCOLOR_Translucent:"#00000000";
    property string cCOLOR_BLACK:"#434343";
    property string cCOLOR_BLACK_DEEP:"#222222";
    property string cCOLOR_ORANGE:"#2da9d3";

    property string cCOLOR_WHITE: "#d2d2d2";
    property string cCOLOR_WHITE2: "#a1a1a1";
    property string cCOLOR_WHITE_DEEP:"#FFFFFF";
    property string cCOLOR_GRAY: "#a1a1a1"
    property string cCOLOR_GRAY_DEEP: "#434343"
    readonly property string translucent: "#00000000";
    readonly property string black: "#434343";
    readonly property string white: "#d2d2d2";
    readonly property string gray: "#a1a1a1";
    readonly property string blue: "#2da9d3";
    readonly property string red: "#FF0000";
    readonly property string green: "#00FF00";
    readonly property string white_pure: "#FFFFFF";
    readonly property string bg_color: "#FFFFFF";
    readonly property string black_deep: "#222222";
    readonly property string orange:"#a8ff00";

    property int rRADIUS: 5;

    property string nameFilter_game: "*.exe ";
    property string nameFilter_video: "*.mp4 *.mkv *.rm *.rmvb *.wmv *.avi *.3gp *.flv *.mpeg *.vod *.mov *.dat *.m4v *.asf *.vob *.asx *.mpg *.mpe "
    property string nameFilter_picture: "*.jpg *.png *.bmp  "
    property var vNAME_FILTERS: {"game":nameFilter_game,"video":nameFilter_video,"picture":nameFilter_picture};

    property int titleBar_Height: 70;
    property int tabBar_Height: 30;

    property int bottomBar_Height: 2;
    property int dialog_height: height-bottomBar_Height;
    property int client_width:width;
    property int client_height: height-titleBar_Height-bottomBar_Height;
    property bool connect: piService.connect;
    property bool controllerPlugin: piService.controllerPlugin;
    property var webView: null;
    property int upgradeFlag: versionModel.piPlayVersion.upgradeFlag;
    property int fmUpgradeFlag: versionModel.firmwareVersion.upgradeFlag;
    property bool isLogin: false;
    property string nick_name: "";
    property string access_token: ""
    readonly property string base_url: "http://piserver.pimaxvr.com:8000/piplay_web/#"
//    property string base_http: "http://118.89.106.249:8080/piplay"
    readonly property string base_http: "http://piserver.pimaxvr.com:17731/piplay_v3/"

    property int mMargin1: 12;
    property int mMargin2: 24;
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

    SystemTrayIcon {
        id:trayIcon;
        visible: true
        iconSource: "qrc:/resource/icon.png"
        tooltip:connect?"PiPlay":qsTr("小派VR设备未连接");
        onActivated: {
            console.log("trayIcon onActivated,",reason);
            showWindow();
        }
        menu: Menu {
            MenuItem {
                text:qsTr("打开PiPlay");
                onTriggered: {
                    showWindow();
                }
            }
            MenuItem {
                text:qsTr("方向校准");
                enabled: connect
                onTriggered: {
                    piService.resetEyePose();
                    statisticsModel.click("btnDirection");
                }
            }
            MenuItem {
                text:qsTr("手柄配对");
                enabled: false;
                onTriggered: {
                }
            }
            MenuItem {
                text:qsTr("设置");
                onTriggered: {
                    createDialog(settingDialog);
                    statisticsModel.loadPage("settingDialog");
                }
            }
            MenuItem {
                text:qsTr("退出");
                iconName: "edit-undo"
                iconSource:"qrc:/resource/trayicon-close.png"
                onTriggered: {
                    statisticsModel.exit();
                    Qt.quit();
                }
            }
        }
        onMessageClicked: showWindow();
    }
    onConnectChanged: {
        if(connect){
            showTrayMessage(qsTr("小派VR设备已连接"));
            console.log("onConnectChanged",connect,piService.modeType);
            resourceModel.launchPihome(piService.modeType==0&&!piService.switchingMode);
            versionModel.checkFirmwareVersion(piService.fmBuildVersion,settingModel.betaTest,piService.product_name);
            statisticsModel.hmdConnected(piService.hmd_name,piService.serial_number,piService.fmVersion,piService.modeType,piService.hmdConfigInfo());
        }else{
            showTrayMessage(qsTr("小派VR设备未连接"));
            resourceModel.launchPihome(false);
            statisticsModel.hmdDisconnect(piService.hmd_name,piService.serial_number);
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
    onUpgradeFlagChanged: {
        if(upgradeFlag==1){
            if(versionCheckingDialog.item&&versionCheckingDialog.item.visible){
                return;
            }
            showUpgradeConfirmDialog();
        }
    }
    onFmUpgradeFlagChanged: {
        if(fmUpgradeFlag==1){
            if(versionCheckingDfuDialog.item&&versionCheckingDfuDialog.item.visible){
                return;
            }
            showVersionCheckingDFUDialog();
        }
    }
    onClosing: {
        console.log("ApplicationWindow onClosing...");
        close.accepted = false;
        if((settingModel.closeToNotibar&&trayIcon&&trayIcon.available)){
            showQuitPrompt();
        }else{
            showQuitPrompt();
        }
    }
    onAccess_tokenChanged: {
        if(access_token==""){
            resourceModel.clearBoughtResource();
        }else{
            resourceModel.searchBoughtResource(access_token);
        }
    }
    onYChanged: {
        if(y<-(titleBar_Height-tabBar_Height-5)){
            y=-(titleBar_Height-tabBar_Height-5);
        }
    }

    SettingModel{
        id:settingModel;
    }
    PiService{
        id:piService
    }
    ResourceModel{
        id:resourceModel
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
    StatisticsModel{
        id:statisticsModel;
    }
    Timer{
        id:checkPiService;
        interval: 5000;
        repeat: false;
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
            resourceModel.checkSteamGame();
            resourceModel.checkOculusGame();
            statisticsModel.postEventData()

        }
    }
    FontLoader { id: webFont; name:"Noto Sans CJK SC DemiLight"}
    FontLoader { id: webFont_bold; name:"Noto Sans CJK SC Bold"}
    Component.onCompleted: {
        checkPiService.start();
        checkResouce.start();
        mainWindow.requestActivate()
        if(trayIcon&&trayIcon.available){
            trayIcon.show();
        }
        hwObject.initLevelInfo(settingModel.language);
        hwObject.checkPcConfigure();
        versionModel.checkPiPlayVersion(settingModel.buildVersion,settingModel.piplayBetaTest);
        if(versionModel.release_note(settingModel.version)!=""){
            showVersionUpgradePromptDialog();
        }
        if(connect){
            versionModel.checkFirmwareVersion(piService.fmBuildVersion,settingModel.betaTest,piService.product_name);
        }
        statisticsModel.start();
    }

    header:Rectangle {//Customize TitleBar
        id: mainTitle
        width: mainWindow.width-2
        x:1;
        height: titleBar_Height
        color: black
        radius: 5;
        focus: true;
        property bool fullScreen:false;
        onFullScreenChanged: {
            if(fullScreen){
                mainWindow.showMaximized();
                btnNormal.visible = true;
                btnMax.visible = false;
                statisticsModel.click("Maximized");
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
            color: translucent;
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
                    mainTitle.fullScreen=!mainTitle.fullScreen;
                }
            }

            ImageButton {//Close Button
                id: btnClose
                anchors.right: parent.right;anchors.rightMargin: mMargin1;
                anchors.verticalCenter: parent.verticalCenter;
                image_normal: "qrc:/resource/quit-n.png"
                mouseArea.onClicked: {
                    (settingModel.closeToNotibar&&trayIcon&&trayIcon.available)?mainWindow.hide():showQuitPrompt();
                }
            }
            ImageButton {//full Screen Button
                id: btnMax
                anchors.right: btnClose.left;anchors.rightMargin: mMargin1;
                anchors.verticalCenter: parent.verticalCenter;
                image_normal: "qrc:/resource/max-n.png"
                mouseArea.onClicked: {
                    mainTitle.fullScreen=true;
                }
            }
            ImageButton {//normal button,default hidden
                id: btnNormal
                visible: false
                anchors.right: btnClose.left;anchors.rightMargin: mMargin1;
                anchors.verticalCenter: parent.verticalCenter;
                image_normal: "qrc:/resource/normal-n.png"
                mouseArea.onClicked: {
                    mainTitle.fullScreen=false;
                }
            }
            ImageButton {//minimized Button
                id: btnMin
                anchors.right: btnMax.left;anchors.rightMargin: mMargin1;
                anchors.verticalCenter: parent.verticalCenter;
                image_normal: "qrc:/resource/min-n.png"
                mouseArea.onClicked: {
                    mainWindow.showMinimized();
                }
            }

            Rectangle{//连接，设置和登录按钮
                id: btnBg;
                width: bSupportAccount?130:80;height: 25;
                anchors.verticalCenter: parent.verticalCenter
                radius: height/2;
                color: white;
                anchors.right: btnMin.left;anchors.rightMargin: mMargin1;
                ImageButton {//setting
                    id:btnSetting
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: bSupportAccount?parent.horizontalCenter:parent.right;anchors.rightMargin: bSupportAccount?-width/2:10;
                    image_normal: "qrc:/resource/config-n.png"
                    mouseArea.onClicked: {
                        createDialog(settingDialog);
                        statisticsModel.loadPage("settingDialog");
                    }
                    RedPoint{
                        visible: (upgradeFlag>0)||(connect&&fmUpgradeFlag>0);
                    }
                }
                ImageButton {//connection status
                    id:btnConnect
                    anchors.left: parent.left;anchors.leftMargin: 10;
                    anchors.verticalCenter: parent.verticalCenter
                    image_normal: piService.connect?"qrc:/resource/link-n.png":"qrc:/resource/disconnect.png"
                    mouseArea.onClicked: {
                        createDialog(connectDialog);
                        statisticsModel.loadPage("connectDialog");
                    }
                    onHoveredChanged: {
                        if(hovered&&connect&&piService.supportModeSwitch){
                            modeSwitchPopup.visible = true;
                        }
                    }
                }
                ImageButton {//login
                    id:btnLogin
                    anchors.right: parent.right;anchors.rightMargin: 10;
                    anchors.verticalCenter: parent.verticalCenter
                    image_normal: "qrc:/resource/login-n.png"
                    visible: bSupportAccount;
                    RedPoint{
                        visible: !isLogin;
                    }
                    mouseArea.onClicked: {
                        if(!isLogin){
                            showLoginDialog();
                        }else{
                            currentPage = 6;
                        }
                    }
                }
            }
            ImageButton {//refresh web page
                id: btnRefresh
                anchors.right: btnBg.left;anchors.rightMargin: mMargin2;
                anchors.verticalCenter: parent.verticalCenter;
                enabled:webView!=null
                image_normal: enabled?"qrc:/resource/refresh-n.png":"qrc:/resource/refresh-d.png"
                mouseArea.onClicked: {
                    if(webView){
                        webView.reload();
                    }else if(currentPage==3){
                        stackView3.clear();
                        stackView3.push(myLibrary);
                    }
                }
            }
            ImageButton {//forward web page
                id: btnForward
                anchors.right: btnRefresh.left;anchors.rightMargin: mMargin2;
                anchors.verticalCenter: parent.verticalCenter;
                enabled:webView!=null&&webView.canGoForward
                image_normal: enabled?"qrc:/resource/front-n.png":"qrc:/resource/front-d&c.png"
                image_pressed: "qrc:/resource/front-d&c.png"
                mouseArea.onClicked: {
                    if(webView!=null){
                        webView.goForward();
                    }
                }
            }
            ImageButton {//go back web page
                id: btnBackPage
                anchors.right: btnForward.left;anchors.rightMargin: mMargin2;
                anchors.verticalCenter: parent.verticalCenter;
                enabled:webView!=null&&webView.canGoBack
                image_normal: enabled?"qrc:/resource/back-n.png":"qrc:/resource/back-d&c.png"
                image_pressed: "qrc:/resource/back-d&c.png"
                mouseArea.onClicked: {
                    if(webView!=null){
                        webView.goBack();
                    }
                }
            }
            Button {//BTN VR
                id: btnVR;
                width: visible?txt.width+10:0;
                height: txt.height-5;
                anchors.right: btnMin.left;anchors.rightMargin: 5;
                anchors.verticalCenter: parent.verticalCenter;
                visible: resourceModel.pihomeIsExist()&&piService.modeType==0&&connect

                background:Rectangle{
                    id:bg
                    radius: 3;
                    color: (btnVR.hovered||mouseAreaVR.pressed)?white:gray
                }
                MyLabel{
                    id:txt
                    anchors.centerIn: parent;
                    color: black
                    font.pixelSize: 12;
                    font.family: webFont_bold.name;
                    text: "VR";
                }
                MyMouseArea{
                    id:mouseAreaVR
                    anchors.fill: parent;
                    hoverEnabled: true;
                    cursorShape: (parent.hovered||pressed)?Qt.PointingHandCursor:Qt.ArrowCursor
                    onClicked: {
                        resourceModel.launchPihome(true);
                    }
                }
            }
        }

        Rectangle{//tabBar
            width: parent.width
            height: tabBar_Height;
            anchors.top: titleBar.bottom;
            color: "#d2d2d2"
            SwitchButton{//home
                id:btnHome
                anchors.right: btnGame.left;anchors.rightMargin: 20;
                anchors.verticalCenter: parent.verticalCenter;
                checked:currentPage==0;
                text: qsTr("首页");
                mouseArea.onClicked: {
                    currentPage = -1;//为了刷新页面
                    currentPage = 0;
                }
            }
            SwitchButton{//Video
                id:btnVideo
                anchors.centerIn: parent;
                checked:currentPage==2;
                text: qsTr("视频")
                mouseArea.onClicked: {
                    currentPage = -1;
                    currentPage = 2;
                }
            }
            SwitchButton{//game
                id:btnGame;
                anchors.right: btnVideo.left;anchors.rightMargin: 20;
                anchors.verticalCenter: parent.verticalCenter;
                checked:currentPage==1;
                text: qsTr("游戏");
                mouseArea.onClicked: {
                    currentPage = -1;
                    currentPage = 1;
                }
            }
            SwitchButton{//my resources
                id:btnLibrary
                anchors.left: btnVideo.right;anchors.leftMargin: 20;
                anchors.verticalCenter: parent.verticalCenter;
                checked:currentPage==3;
                text: qsTr("我的资源");
                mouseArea.onClicked: {
                    currentPage = 3;
                    statisticsModel.loadPage("library");
                }
                Rectangle{
                    height: subText.height;width: height;
                    anchors.top: parent.top;anchors.topMargin: 0;
                    anchors.right: parent.right;anchors.rightMargin: 0;
                    radius: height/2;
                    color:"#7F0000";
                    visible: resourceModel.dlingCount>0
                    Text{
                        id: subText;
                        color:white;
                        text:resourceModel.dlingCount
                        anchors.centerIn: parent;
                        font.pixelSize: 8;
                    }
                }
            }
            SwitchButton{//个人中心
                id:personalInfo;
                anchors.left: btnLibrary.right;anchors.leftMargin: 20;
                anchors.verticalCenter: parent.verticalCenter;
                checked:currentPage==6;
                visible: isLogin&&bSupportAccount;
                text: qsTr("个人中心");
                mouseArea.onClicked: {
                    currentPage = -1;
                    currentPage = 6;
                }
                onVisibleChanged: {
                    if(currentPage==6&&!visible){
                        currentPage = 0;
                    }
                }
            }
        }

        Button{//btnLogo
            id:btnLogo;
            anchors.left: parent.left;anchors.leftMargin: titleBar_Height-tabBar_Height;
            anchors.top: parent.top;
            width: imageLogo.width;
            height: imageLogo.height;
            background:Rectangle{
                color: translucent
            }
            Image{
                id:imageLogo;
                source:"qrc:/resource/PiPlay_logo.png";
            }
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
            webView = null;
            if(currentIndex==0){
                showOnlinePage(stackView0,onlineHome);
            }else if(currentIndex==1){
                showOnlinePage(stackView1,onlineGame);
            }else if(currentIndex==2){
                showOnlinePage(stackView2,onlineVideo);
            }else if(currentIndex==3){
                stackView3.clear();
                stackView3.push(myLibrary);
            }else if(currentIndex==6){
                showOnlinePage(stackView6,myProfile);
            }
        }


        MyStackView{
            id:stackView0;
            Component.onCompleted: {
                stackView0.push(onlineHome);
                webView = stackView0.get(stackView0.depth-1);
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
        MyStackView {
            id: stackView5
        }
        MyStackView {
            id: stackView6
        }
    }
    function showOnlinePage(view,component){
        view.clear();
        view.push(component);
        webView = view.get(view.depth-1);
    }

    Component{//我的资源
        id:myLibrary;
        LibraryDialog{
            width:stackView3.width;height: stackView3.height;
            clip: true;
        }
    }
    Component{//设置
        id:settingDialog;
        SettingDialog{
        }
    }
    Component{//连接
        id:connectDialog;
        ConnectDialog{
        }
    }
    Component{//游戏
        id: onlineHome
        OnlineBrowser{
            url:base_url+"/home?"+settingModel.getLangAndLcid();
            clip: true;
        }
    }
    Component{//游戏
        id: onlineGame
        OnlineBrowser{
            url: base_url+"/list?type=game&filter=all&select=score-true&"+settingModel.getLangAndLcid();
            clip: true;
        }
    }
    Component{//视频
        id: onlineVideo
        OnlineBrowser{
//            anchors.top: parent.top;anchors.topMargin: -70;
//            height: parent.height+70;
//            url:"https://www.viveport.com/"
            url:base_url+"/list?type=video&filter=all&select=score-true&"+settingModel.getLangAndLcid();
            clip: true;
        }
    }
    Component{//我的资料
        id: myProfile
        OnlineBrowser{
            url:base_url+"/mine/myProfile?"+settingModel.getLangAndLcid();
            clip: true;
        }
    }

    Loader{
        id: alertPopup;
    }

    function showAlertPopup(alert){
        if(alertPopup.source==""){
            alertPopup.source = "MyAlertPopup.qml";
        }
        alertPopup.item.text = alert;
        alertPopup.item.show();
        alertPopup.item.raise();
    }

    function showTrayMessage(message){
        if(trayIcon&&trayIcon.available){
            console.log("ShowTrayMessage,",message);
            trayIcon.showMessage(message,"",SystemTrayIcon.Information,1000);
        }
        showAlertPopup2(message);
    }
    function showAlertPopupAtBottom(message){
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
        var dialog = createDialog(confirmDialog);
        dialog.text = message;
        dialog.callback = callback;
        return dialog;
    }

    function showConfirmDialog(message){
        showConfirmDialog2(message,null);
    }


    function showDisconnectMessage(){
        showConfirmDialog(qsTr("小派设备未连接!<br/>提示：请先确认USB和HDMI连接正常"));
    }

    function showWindow(){
        if(mainTitle.fullScreen){
            mainWindow.showMaximized();
        }else{
            mainWindow.showNormal();
        }
        mainWindow.raise()
        mainWindow.requestActivate()
    }

    function showDialog(dialogId,qmlFile){
        console.log("showDialog",qmlFile);
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

    Loader{
        id:versionUpgradingDfuDialog;
    }
    function showVersionUpgradeDFUDialog(){
        showDialog(versionUpgradingDfuDialog,"VersionUpgrading_DFU.qml");
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
        x: mainWindow.x+btnConnect.x+btnBg.x;
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
        if(!piService.supportModeSwitch){
            return false;
        }
        if((playMode=="game"||playMode=="vr"||playMode=="view"||playMode=="picture")&&piService.modeType==1){
            //需要切换到小派玩家模式
            showSwithModePrompt(qsTr("游戏或观看全景影视需切换至小派玩家模式,<br/>是否立即切换?"),0);
            return true;
        }else if((playMode=="2D"||playMode=="2d"||playMode=="3D")&&piService.modeType==0){
            //需要切换到普通观影模式
            showSwithModePrompt(qsTr("观看2D/3D影视需切换至普通观影模式,<br/>是否立即切换?"),1);
            return true;
        }
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
        showMessageDialog2(qsTr("立即安装"),qsTr("暂不安装"),qsTr("此游戏为Steam游戏，请安装Steam VR软件"),function(){
            Qt.openUrlExternally(qsTr("http://bbs.pimaxvr.com/read.php?tid=571&fid=8"));
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
        showMessageDialog(qsTr("是否确认退出PiPlay?"),function(){
            statisticsModel.exit();
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
        MyCheckBox{
            id:checkbox;
            text:qsTr("同时删除相关文件");
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: parent.verticalCenter;
            checked: false;
            fontSize: 12;
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
    property Component loginDialogComponent: LoginDialog {
        onClosing: destroy()
    }
    function showLoginDialog(callback,profile,from){
        var dialog = loginDialogComponent.createObject(mainWindow);
        if(profile){
            dialog.currentWebView.profile = profile;
        }
        if(from){
            dialog.from = from;
        }
        dialog.callback = callback;
        return dialog;
    }
    property Component browserDialogComponent: BrowserDialog {
        onClosing: destroy()
    }
    function createBrowserDialog(profile) {
        var newDialog = browserDialogComponent.createObject(mainWindow)
        newDialog.currentWebView.profile = profile
        return newDialog
    }
    function showResourceDetail(resource_id,from){
        currentPage = 0;
        var url = base_url+"/detail?id="+resource_id+"&"+settingModel.getLangAndLcid()+"&from="+from;
        webView.url = url;
    }
}

