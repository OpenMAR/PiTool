import QtQuick 2.7
import QtQuick.Controls 2.0

Button{//下载试用
    id:btnDownload;
    property var localGame: null;
    property var onlineGame: null;
    property bool bGame: true;
    width:290;height: 60;
    background: Rectangle{
        radius: 5
        color:parent.pressed?black_deep:black
    }
    Rectangle{//progress
        color:blue;
        radius: 5
        width: localGame==null?0:parent.width*localGame.readBytes/localGame.totalBytes
        height: parent.height;
        visible: localGame&&localGame.status!=ResourceItem.STATUS_LOCAL;
    }

    MyLabel{
        text:{
            if(localGame!=null){
                if(localGame.status==ResourceItem.STATUS_LOCAL){
                    return qsTr("打开");
                }else if(localGame.status==ResourceItem.STATUS_READY_DOWNLOAD){
                    return qsTr("已暂停");
                }else if(localGame.status==ResourceItem.STATUS_INSTALLING){
                    return qsTr("正在安装...");
                }else{
                    return qsTr("正在下载...");
                }
            }else{
                return (bGame?qsTr("下载试玩"):qsTr("下载试看"))+"("+(onlineGame==null?"":onlineGame.totalBytesFmt)+")";
            }
        }
        font{pixelSize: 20;bold:true}
        color:gray
        anchors.centerIn: parent;
    }
    MouseArea{
        anchors.fill: parent;
        hoverEnabled: true;
        cursorShape: (hovered||pressed)?Qt.PointingHandCursor:Qt.ArrowCursor
        onClicked: {
            if(localGame!=null){
                if(localGame.status==ResourceItem.STATUS_LOCAL){//打开
                    if(!piService.connect){
                        showDisconnectMessage();
                        return;
                    }
                    resourceModel.openItemById(localGame.id);
                }else if(localGame.status==ResourceItem.STATUS_READY_DOWNLOAD){
                    resourceModel.startDownload(localGame.id);
                }else if(localGame.status==ResourceItem.STATUS_DOWNLOADING){
                    resourceModel.stopDownload(localGame.id);
                }
            }else{
                resourceModel.downloadResouce(onlineGame.id,onlineGame.thirdId,onlineGame.url,onlineGame.imageUrl,onlineGame.title,onlineGame.type);
                localGame = resourceModel.getItem(onlineGame.id,onlineGame.thirdId);
            }
        }
    }
}


