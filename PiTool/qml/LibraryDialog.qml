import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2
import pimaxvr.ResourceItem 1.0
import QtQuick.Dialogs 1.2

Rectangle{
    property int topMARGIN : 20
    property int mMARGIN:20;
    color:white_pure;
    width: parent.width
    height: parent.height
    property int currentIndex: 0;

    Rectangle{
        id:sideLeft;
        width: tabDownloading.width+80;
        height: parent.height;
        color: black;
        anchors.left: parent.left;
        anchors.top: parent.top;
        MyTabButton{//已下载（本地资源)
            id:tabLocal
            text: qsTr("已下载")
            checked:currentIndex==0;
            anchors.right: parent.right;anchors.rightMargin: 40;
            anchors.top: parent.top;anchors.topMargin: topMARGIN+mMARGIN;
            mouseArea.onClicked: {
                currentIndex = 0;
            }
            txt.color: checked?blue:white
        }
        MyTabButton{//正在下载
            id:tabDownloading
            visible: false
            text: qsTr("正在下载");//+"("+resourceModel.dlingCount+")"
            checked:currentIndex==1;
            anchors.top: tabLocal.bottom;anchors.topMargin: mMARGIN;
            anchors.right: tabLocal.right;
            mouseArea.onClicked: {
                currentIndex = 1;
            }
            txt.color: checked?blue:white
            RedPoint{
                visible:resourceModel.dlingCount>0;
            }
        }

        MyTabButton{//待升级
            id:tabUpgrade
            visible:false;
            text: qsTr("待升级")
            checked:currentIndex==2;
            anchors.right: tabLocal.right;
            anchors.top: tabDownloading.bottom;anchors.topMargin: mMARGIN;
            mouseArea.onClicked: {
                currentIndex = 2;
            }
            txt.color: checked?blue:white
        }
        MyTabButton{//我的购买
            id:tabBought
            text: qsTr("我的购买")
            checked:currentIndex==3;
            anchors.right: tabLocal.right;
            anchors.top: tabUpgrade.bottom;anchors.topMargin: mMARGIN;
            mouseArea.onClicked: {
                currentIndex = 3;
            }
            txt.color: checked?blue:white
            visible: false//isLogin&&bSupportAccount;
            onVisibleChanged: {
                if(!visible&&currentIndex==3){
                    currentIndex = 0;
                }
            }
        }
        TextButton{
            id: btnOpenLocal
            text: qsTr("本地打开")
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.bottom: parent.bottom;anchors.bottomMargin: 40;
            round:true
            plain: true;
            size:large
            txt.color: white
            width: btnImportLocal.width;
            mouseArea.onClicked: {
                if(!piService.connect){
                    showDisconnectMessage();
                    return;
                }
                fileDialog.onlyOpen = true;
                if(piService.modeType==1){
                    fileDialog.nameFilters = [qsTr("文件")+"("+nameFilter_video+")"];
                }else{
                    fileDialog.nameFilters = [qsTr("文件")+"("+nameFilter_game+nameFilter_video+nameFilter_picture+")"];
                }
                fileDialog.open();
            }
        }
        TextButton{
            id: btnImportLocal
            text: qsTr("本地导入")
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.bottom: btnOpenLocal.top;anchors.bottomMargin: mMARGIN;
            round:true;
            plain: true;
            size:large
            txt.color: white
            mouseArea.onClicked: {
                fileDialog.onlyOpen = false;
                fileDialog.nameFilters = [qsTr("文件")+"("+nameFilter_game+nameFilter_video+nameFilter_picture+")"];
                fileDialog.open();
            }
        }
        TextButton{
            id: btnExchangeProduct
            text: qsTr("激活产品")
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.bottom: btnImportLocal.top;anchors.bottomMargin: mMARGIN;
            round:true;
            size:large
            plain: true;
            txt.color: white
            visible: false//bSupportAccount
            mouseArea.onClicked: {
                loginCheck();
            }
        }
    }

    FileDialog {
        property bool onlyOpen: false;
        id: fileDialog
        selectFolder:false
        selectMultiple:!onlyOpen;
        title: qsTr("请选择文件")
        onAccepted: {
            fileDialog.close();
            if(onlyOpen){
                if(!piService.connect){
                    showDisconnectMessage();
                    return;
                }
                resourceModel.playResource(fileDialog.fileUrl);
            }else{
                console.log("import file,",fileDialog.folder);
                loadLocalResoures(fileDialog.fileUrls);
            }
        }
        onRejected: {
            console.log("Canceled")
        }
        Component.onCompleted: visible = false;
    }
    Item {
        id: main
        width:parent.width-sideLeft.width;
        height:parent.height;
        anchors.top: parent.top;
        anchors.left: sideLeft.right;
        property int currentIndex:parent.currentIndex;
        onCurrentIndexChanged:{
            var pageName=['library_local','library_downloading','library_upgrading','library_bought'];
            library.dialogType = currentIndex;
        }
        LibraryList{
            id:library;
            width: parent.width;height: parent.height;
        }
    }
    function showConfirm1(){
        showAlertPopup(qsTr("路径中仅可存在英文、阿拉伯数字或下划线"));
    }
    property Component exchangeCodeDialog: ExchangeCodeDialog {
        onClosing: destroy()
    }

    function loginCheck(){
        if(!isLogin){
            showLoginDialog(showExchangeCodeDialog);
            return;
        }
        //判断登录是否有效
        var request = new XMLHttpRequest();
        request.onreadystatechange=function() {
            if(request.readyState == XMLHttpRequest.DONE) {
                if (request.status == 200) {
                    console.log("Response = " + request.responseText);
                    var response = JSON.parse(request.responseText);
                    if(response.code==8888){
                         showExchangeCodeDialog();
                    }else{
                        showLoginDialog(showExchangeCodeDialog);
                    }
                }
                else {
                    console.log("Status: " + request.status + ", Status Text: " + request.statusText);
                    showLoginDialog(showExchangeCodeDialog);
                }
            }
        }
        var url = base_http+"/user/islogin?access_token="+access_token;
        request.open("get",url, true);
        request.send();
    }
    function showExchangeCodeDialog(){
        createDialog(exchangeCodeDialog);
    }
}
