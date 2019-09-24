import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import pimaxvr.ResourceItem 1.0
import QtGraphicalEffects 1.0

import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtMultimedia 5.8

Rectangle{
    color:cCOLOR_BG
    property var gGRID_WIDTH: 260;
    property var gGRID_HEIGHT:200;
    property var iIMAGE_WIDTH:245;
    property var iIMAGE_HEIGHT:135;
    property int selectedIndex:0;
    property var countPerRow: parseInt(parent.width/gGRID_WIDTH)

    Rectangle{
        width:parent.width;height: parent.height;
        color:parent.color;
        visible: grid.count==0;
        Image{
            id:imageNoResource;
            anchors.top: parent.top;anchors.topMargin: 30;
            anchors.horizontalCenter: parent.horizontalCenter;
            source: "qrc:/resource/none.png";
        }
        MyLabel{
            id:labelNoResource;
            anchors.top: imageNoResource.bottom
            anchors.topMargin: 20;
            anchors.horizontalCenter: parent.horizontalCenter;
            text:qsTr("暂无下载中的内容")
        }
    }

    Rectangle{
        visible: grid.count>0;
        width: parent.width;height: parent.height;
        color: cCOLOR_Translucent
        property int currentBtn:0;
        MyTabButton{
            id:sortName;
            text:qsTr("名称")
            anchors.top: parent.top;
            anchors.right: scrollView.right;anchors.rightMargin: 40;
            showLine:false;
            fontSize:14;
            checked: parent.currentBtn==1;
            mouseArea.onClicked: {
                parent.currentBtn = 1;
                resourceModel.sortDlingList(1);
            }
        }
        MyTabButton{
            text:qsTr("时间")
            anchors.top: parent.top;
            anchors.right: sortName.left;anchors.rightMargin: 10;
            showLine:false;
            fontSize:14
            checked: parent.currentBtn==0;
            mouseArea.onClicked: {
                parent.currentBtn = 0;
                resourceModel.sortDlingList(0);
            }
        }
        MyScrollView{
            id:scrollView;
            width: countPerRow*gGRID_WIDTH+20;
            anchors.top: sortName.bottom;anchors.topMargin: 10;
            height: parent.height-y;
            visible: grid.count>0;
            anchors.horizontalCenter: parent.horizontalCenter;


            GridView{
                id:grid;
                objectName: "ResourceItem"
                width:countPerRow*gGRID_WIDTH;height: contentHeight;
                cellWidth: gGRID_WIDTH;cellHeight: gGRID_HEIGHT;
                anchors.horizontalCenter: parent.horizontalCenter;
                model:resourceModel.dlingList//resourceModel.dlingCount
                delegate: resourceDelegate;
                focus:true
                clip: true;
            }
        }
        Component{
            id:resourceDelegate;
            Button{
                id:cell
                width:grid.cellWidth;
                height:iIMAGE_HEIGHT;
                background: Rectangle{
                    color: cCOLOR_Translucent
                }
                property var resource: model.modelData;
                property var color:(resource.status==ResourceItem.STATUS_DOWNLOADING)?"#FFDE00":cCOLOR_WHITE
                property var totalBytes:resource.totalBytes;
                property var status: resource.status;
                RectangularGlow{
                    anchors.fill: image;
                    glowRadius: 5;
                    cornerRadius:10;
                    spread: 0.4;color:"#7F000000";
                }
                MyImage{
                    id:image
                    source:resource.image_url;
                    anchors.top:parent.top;
                    anchors.horizontalCenter: parent.horizontalCenter;
                    width: iIMAGE_WIDTH;height: iIMAGE_HEIGHT;
                }
                MyProgressBar{
                    id: progressBar
                    height: 5;
                    width: image.width;
                    anchors.top: image.bottom;
                    anchors.horizontalCenter: parent.horizontalCenter;
                    minimumValue: 0
                    maximumValue: resource.totalBytes
                    value: resource.readBytes
                    progressColor: cell.color;
                }

                MyText{
                    id:title
                    text:resource.title;
                    width: btnClose.x-5;
                    anchors.left: progressBar.left;anchors.top: progressBar.bottom;
                    anchors.topMargin: 5
                }
                MyText{
                    id:create_date;
                    text:resource.create_date
                    anchors.left: image.left;anchors.top: title.bottom;
                    anchors.topMargin: 5
                }
                ImageButton {//delete Button
                    id: btnClose
                    anchors.right: image.right;anchors.top: progressBar.bottom;
                    image_normal: "qrc:/resource/delete-n.png"
                    image_hover: "qrc:/resource/delete-h.png"
                    image_pressed: "qrc:/resource/delete-c.png"
                    mouseArea.onClicked: {
                        showDeleteDownloadingDialog(index);
                    }
                }
                Rectangle{//下载进度
                    color:'#cc000000';
                    width:image.width;height: image.height;
                    anchors.centerIn: image
                    visible: parent.hovered?true:false;
                    ImageButton{
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 10
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        iconSource: (resource.status==ResourceItem.STATUS_DOWNLOADING)?"qrc:/resource/pause-h.png":"qrc:/resource/start-n.png"
                    }
                    MyLabel{
                        color:cell.color
                        text:resourceModel.formatBytes(resource.readBytes)+'/'+resourceModel.formatBytes(resource.totalBytes)
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 20
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                    }
                    MyLabel{
                        color:cell.color
                        text:qsTr("预计剩余时间：")+resource.remainTime
                        anchors.top: parent.top;
                        anchors.topMargin: 20
                        anchors.horizontalCenter: parent.horizontalCenter
                        visible:(resource.status==ResourceItem.STATUS_DOWNLOADING)?true:false
                    }
                    MyLabel{
                        color:cell.color
                        text:(resource.status==ResourceItem.STATUS_DOWNLOADING)?resource.percent:qsTr("等待下载...")
                        anchors.centerIn: parent;
                        font.pixelSize: (resource.status==ResourceItem.STATUS_DOWNLOADING)?54:25
                        font.bold: true
                    }
                    MouseArea{
                        anchors.fill: parent;
                        hoverEnabled: true;
                        cursorShape: (hovered||pressed)?Qt.PointingHandCursor:Qt.ArrowCursor
                        onClicked:{
                            if(resource.status==ResourceItem.STATUS_DOWNLOADING){
                                resourceModel.stopDownload(resource.id);
                            }else if(resource.status==ResourceItem.STATUS_READY_DOWNLOAD
                                     ||resource.status==ResourceItem.STATUS_DOWNLOAD_FAILED){
                                resourceModel.startDownload(resource.id);
                            }
                        }
                    }
                }
            }
        }
    }


    function showDeleteDownloadingDialog(index){
        console.log("showDeleteDownloadingDialog");
        showDeleteDialog(qsTr("是否取消下载？"),function(bDeleteFile){
            resourceModel.deleteDling(index,bDeleteFile);
        })
    }
}
