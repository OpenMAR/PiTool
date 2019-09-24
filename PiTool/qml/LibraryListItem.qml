import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import pimaxvr.ResourceItem 1.0
import QtGraphicalEffects 1.0

Rectangle{
    property var iIMAGE_WIDTH:255;
    property var iIMAGE_HEIGHT:147;
    property var resource:null;
    property int dialog: 0;
    color: translucent;

    RectangularGlow{
        anchors.fill: rectBG;
        glowRadius: 5;
        cornerRadius:5;
        spread: 0.3;color:"#1F000000";
    }

    Rectangle{
        id:rectBG;
        width: parent.width-10;
        height: parent.height-20;
        anchors.top:parent.top;anchors.topMargin: 5;
        anchors.horizontalCenter: parent.horizontalCenter;
        clip: true;
        radius: rRADIUS;
        Rectangle{
            id:rectImage;
            width: parent.width;height: iIMAGE_HEIGHT;
            anchors.horizontalCenter: parent.horizontalCenter;
            color:translucent;
            clip: true;
            radius: rRADIUS
            MyImage{
                id:image
                source:resource.image_url;
                anchors.top:parent.top;
                anchors.horizontalCenter: parent.horizontalCenter;
                width: parent.width;height: iIMAGE_HEIGHT;
                MouseArea{
                    id:imageMouseArea
                    anchors.fill: parent;
                    hoverEnabled: true;
                }
            }

            Rectangle{
                color:'#BF000000';
                width:image.width;height: image.height;
                anchors.centerIn: image
                visible: (imageMouseArea.containsMouse||bgMouseArea.containsMouse||(resource.status>0&&resource.status!=ResourceItem.STATUS_ONLINE))?true:false;
                MouseArea{
                    id:bgMouseArea
                    anchors.fill: parent;
                    hoverEnabled: true;
                }
                Rectangle{//下载
                    width:parent.width;height: parent.height;
                    color:translucent
                    visible: resource.status==ResourceItem.STATUS_ONLINE
                    TextButton{
                        id:btnDownload
                        anchors.centerIn: parent
                        text:qsTr("下载")
                        round:true
                        size:large;
                        type:primary;
                        mouseArea.onClicked:{
                            downloadResource(resource.id);
                        }
                    }
                }
                Rectangle{//开始游戏
                    width:parent.width;height: parent.height;
                    color:translucent
                    visible: resource.type=="game" && resource.status==0
                    TextButton{
                        id:btnPlay
                        anchors.centerIn: parent
                        text:qsTr("开始游戏")
                        round:true
                        size:large;
                        type:primary;
                        mouseArea.onClicked:{
                            if(!piService.connect){
                                showDisconnectMessage();
                                return;
                            }

                            if(bNeedSwidthModeType("game")){
                                return;
                            }

                            if((resource.subType=="steam")&&!resourceModel.steamVrIsInstalled()){//||resource.isSteamApp()
                                showInstallSteamVr("");
                                return;
                            }

                            if((resource.subType=="oculus") && !diagnoseHandler.ovrServiceAlived()){
                                showMessageDialog2(qsTr("确认"),qsTr("取消"),qsTr("需要启动Oculus服务，是否现在启动？"),function(){
                                    diagnoseHandler.startServiceByName("OVRService");
                                },function(){
                                    return;
                                }
                                );
                                return;
                            }

                            if(!resourceModel.playGame(resource.id)){
                                showErrorMessage(resource.id);
                            }
                        }
                    }
                    MyLabel{
                        visible: resource.type=="game";
                        font.family: webFont_bold.name;
                        text:{
                            if(!visible){
                                return "";
                            }
                            if(resource.subType=="oculus"){
                                return qsTr("需启动Oculus Home");
                            }else if(resource.subType=="steam"||resource.isSteamApp()){
                                return qsTr("需启动SteamVR");
                            }
                            return "";
                        }
                        anchors.top: btnPlay.bottom;anchors.topMargin: 10;
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: white
                    }
                }
                Rectangle{//打开图片
                    width:parent.width;height: parent.height;
                    color:translucent
                    visible: resource.type=="picture" && resource.status==0
                    TextButton{
                        id:btnPicture
                        anchors.centerIn: parent
                        text:qsTr("打开图片")
                        visible: resource.type=="picture";
                        round:true
                        size:large;
                        type:primary;
                        mouseArea.onClicked:{
                            if(!piService.connect){
                                showDisconnectMessage();
                                return;
                            }
                            if(bNeedSwidthModeType("picture")){
                                return;
                            }
                            if(!resourceModel.playVideo(resource.id)){
                                showErrorMessage(resource.id);
                            }
                        }
                    }
                }
                Rectangle{//播放视频
                    width:parent.width;height: parent.height;
                    color:translucent
                    visible: resource.type=="video" && resource.status==0
                    TextButton{
                        id:btnPlayVideo
                        anchors.verticalCenter: parent.verticalCenter;
                        anchors.right: parent.horizontalCenter;anchors.rightMargin: 5;
                        text:qsTr("全景")
                        round:true
                        size:medium;
                        type:primary;
                        mouseArea.onClicked:{
                            if(!piService.connect){
                                showDisconnectMessage();
                                return;
                            }
                            if(bNeedSwidthModeType("vr")){
                                return;
                            }
                            if(!resourceModel.playVideo(resource.id)){
                                showErrorMessage(resource.id);
                            }
                        }
                    }
                    TextButton{
                        id:btnPlay2DVideo
                        anchors.verticalCenter: parent.verticalCenter;
                        anchors.left: parent.horizontalCenter;anchors.leftMargin: 5;
                        text:qsTr("2D/3D")
                        round: true
                        size:medium;
                        type:primary;
                        mouseArea.onClicked:{
                            if(!piService.connect){
                                showDisconnectMessage();
                                return;
                            }
                            if(bNeedSwidthModeType("2D")){
                                return;
                            }
                            if(!resourceModel.play2DVideo(resource.id)){
                                showErrorMessage(resource.id);
                            }
                        }
                    }
                }
                Rectangle{//正在下载
                    width:parent.width;height: parent.height;
                    color:translucent
                    visible: resource.status>0&&resource.status!=ResourceItem.STATUS_ONLINE
                    MyLabel{//下载字节数
                        color:white
                        text:qsTr("下载进度：")+resourceModel.formatBytes(resource.readBytes)+'/'+resourceModel.formatBytes(resource.totalBytes)
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 28
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    MyLabel{
                        color:white
                        text:qsTr("预计剩余时间：")+resource.remainTime
                        anchors.top: parent.top;
                        anchors.topMargin: 30
                        anchors.horizontalCenter: parent.horizontalCenter
                        visible:(resource.status==ResourceItem.STATUS_DOWNLOADING)?true:false
                    }
                    MyLabel{//下载百分比
                        color:white
                        text:resource.percent
                        anchors.centerIn: parent;
                        visible: resource.status==ResourceItem.STATUS_DOWNLOADING
                                &&!(imageMouseArea.containsMouse||bgMouseArea.containsMouse)
                        font.pixelSize: 24
                        font.bold: true
                    }
                    MyLabel{//状态
                        color:white
                        text:(resource.status==ResourceItem.STATUS_INSTALLING)?qsTr("正在安装..."):qsTr("已暂停")
                        visible: resource.status!=ResourceItem.STATUS_DOWNLOADING
                                 &&!(imageMouseArea.containsMouse||bgMouseArea.containsMouse)
                        anchors.centerIn: parent;
                        font.pixelSize: 24
                        font.bold: true
                    }
                    MyProgressBar{//下载进度条
                        id: progressBar
                        height: 5;
                        width: parent.width;
                        anchors.bottom: parent.bottom;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        minimumValue: 0
                        maximumValue: resource.totalBytes
                        value: resource.readBytes
                        progressColor: blue;
                        visible: resource.status>0&&resource.status!=ResourceItem.STATUS_ONLINE
                    }
                    TextButton{
                        anchors.centerIn: parent
                        text:qsTr("继续")
                        round:true
                        size:large;
                        type:primary;
                        visible:(imageMouseArea.containsMouse||bgMouseArea.containsMouse||mouseArea.containsMouse)
                                &&(resource.status==ResourceItem.STATUS_READY_DOWNLOAD||resource.status==ResourceItem.STATUS_DOWNLOAD_FAILED);
                        mouseArea.onClicked:{
                            resourceModel.startDownload(resource.id);
                        }
                    }
                    TextButton{
                        anchors.centerIn: parent
                        text:qsTr("暂停")
                        round:true
                        size:large;
                        type:primary;
                        visible:(imageMouseArea.containsMouse||bgMouseArea.containsMouse||mouseArea.containsMouse)
                                &&resource.status==ResourceItem.STATUS_DOWNLOADING;
                        mouseArea.onClicked:{
                            resourceModel.stopDownload(resource.id);
                        }
                    }
                }
            }
        }

        MyText{
            id:title
            text:resource.title;
            anchors.left: rectImage.left;anchors.leftMargin: 5;
            anchors.top: rectImage.bottom;anchors.topMargin: 5;
            width: btnClose.x-10;
            elide: Text.ElideRight
            color: titleArea.containsMouse?blue:black;
            font.pixelSize: 14;
            MyMouseArea{
                id:titleArea;
                visible: resource.id>=1000000&&resource.id<9999999
                onClicked: {
                    showResourceDetail(resource.id,"libray");
                }
            }
            Rectangle{
                width:title.contentWidth;
                height:1
                visible:titleArea.containsMouse&&titleArea.visible
                color:titleArea.containsMouse?blue:black;
                anchors.bottom: parent.bottom
            }
        }
        MyText{
            id:create_date
            text:resource.create_date
            anchors.left: title.left;
            anchors.top: title.bottom;anchors.topMargin: 5
            color: gray
            font.pixelSize: 12;
        }
        ImageButton {//delete Button
            id: btnClose
            anchors.right: rectImage.right;
            anchors.top: rectImage.bottom;anchors.topMargin: 5;
            image_normal: "qrc:/resource/delete-n.png"
            visible: (resource.status!=ResourceItem.STATUS_ONLINE && resource.subType!="steam" && resource.subType!="oculus")
            mouseArea.onClicked: {
                if(resource.subType=="steam"){
                    showAlertPopup(qsTr("请在Steam App中卸载该游戏"));
                    return;
                }
                if(resource.subType=="oculus"){
                    showAlertPopup(qsTr("请在Oculus home中卸载该游戏"));
                    return;
                }
                showDeleteResourceDialog(resource.id,qsTr("从列表中移除所选内容?"));
            }
        }
        MyLabel{
            text:resource&&resource.subType=="oculus"?"Oculus":"";
            anchors.top: create_date.top;
            anchors.right: rectImage.right;
            color: black;
            font.pixelSize: 12;
        }
        MySteamButton{
            id:btnSteamVR
            anchors.top: create_date.top;
            anchors.right: rectImage.right;
            visible: resource&&resource.subType=="steam"
        }
    }


    function showDeleteResourceDialog(resId,message){
        console.log("showDeleteResourceDialog");
        showDeleteDialog(message,function(bDeleteFile){
            resourceModel.deleteResource(resId,bDeleteFile);
        })
    }
    function showDeleteResourceFromListlibDialog(resId,message){
        console.log("showDeleteResourceFromListlibDialog");
        showDeleteListLibDialog(message,function(bDeleteFile){
            if(bDeleteFile){
                resourceModel.deleteResource(resId,false);
            }
        })
    }

    function showErrorMessage(resId){
        console.log("showErrorMessage",resourceModel.getErrorCode());
        if(resourceModel.getErrorCode()==1){
            showDeleteResourceFromListlibDialog(resId,qsTr("该内容已被移动或删除，无法打开"));
        }else{
            showAlertPopup(resourceModel.getError());
        }
    }

    function downloadResource(id){
        var request = new XMLHttpRequest();
        request.onreadystatechange=function() {
            if(request.readyState == XMLHttpRequest.DONE) {
                if (request.status == 200) {
                    console.log("Response = " + request.responseText);
                    var response = JSON.parse(request.responseText);
                    if(response.code==8888){
                        resourceModel.downloadResouce(resource.id,"",resource.type,resource.subType,response.data.download_url,resource.image_url,resource.title,resource.desc);
                    }else{
                        showAlertPopup(response.message);
                    }
                }
                else {
                    showAlertPopup(qsTr("下载失败"));
                }
            }
        }
        var url = base_http+"/product/download?access_token="+access_token
            +"&id="+id+"&"+settingModel.getLangAndLcid();
        request.open("get",url, true);
        request.send();
//
    }
}

