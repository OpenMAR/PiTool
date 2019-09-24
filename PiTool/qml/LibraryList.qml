import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import pimaxvr.ResourceItem 1.0

import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtMultimedia 5.8
import QtGraphicalEffects 1.0

Rectangle{
    visible: true;
    property int parent_width: parent.width;
    property int parent_height:parent.height
    property int dialogType: 0;//窗口类型,0:已下载，1：正在下载，2：待升级, 3:我的购买
    property bool show: false;
    property int gGRID_WIDTH: 275;
    property int gGRID_HEIGHT:220;
    property int comboBoxWidth:80;
    property int selectedIndex: 0;
    property int countPerRow: parseInt(width/gGRID_WIDTH);
    property int resCount: resourceModel.resCount;
    property int dlingCount: resourceModel.dlingCount;
    readonly property int dialog_bought: 3;

    Component.onCompleted: {
        searchResource();
    }
    onResCountChanged: {
        console.log("onResCountChanged,count="+resCount);
        searchResource();
    }
    onDlingCountChanged: {
        console.log("onDlingCountChanged,count="+dlingCount);
        searchResource();
    }
    onDialogTypeChanged: {
        inputSearch.text="";
        comboSort.currentIndex = 0;
        comboType.currentIndex = 0;
        searchResource();
    }
    Rectangle{//titlebar
        id:titleBar;
        width: 803//grid.width;
        anchors.top: parent.top;anchors.topMargin: 25;
        height: 30//50;
        anchors.horizontalCenter: parent.horizontalCenter;

        MyComboBox {
            id: comboType
            visible: false
            width:comboBoxWidth
            height: 20
            anchors.verticalCenter: parent.verticalCenter;
            anchors.left: parent.left;
            currentIndex: 0
            model: [qsTr("所有"),qsTr("游戏"),qsTr("视频"),qsTr("图片")]
            isBlack: true;
            onCurrentIndexChanged: {
                searchResource();
            }
        }

        MyComboBox {
            id: comboSort
            borderColor: white
            height: 20
            width:comboBoxWidth+30;
            anchors.bottom: parent.bottom;
            anchors.right: parent.right;anchors.rightMargin: 15;
            currentIndex: 0
            model: dialogType==dialog_bought?[qsTr("购买时间"),qsTr("发布时间"),qsTr("名称")]:[qsTr("使用时间"),qsTr("导入时间"),qsTr("名称")]
            isBlack: true;
            onCurrentIndexChanged: {
                searchResource();
            }
        }
        MyComboBox {
            id: typeSort
            borderColor: white
            height: 20
            width:comboBoxWidth+30;
            anchors.bottom: parent.bottom;
            anchors.right: comboSort.left;anchors.rightMargin: 15;
            currentIndex: 0
            model: [qsTr("所有游戏"),qsTr("SteamVR"),qsTr("Oculus"),qsTr("本地导入")]
            isBlack: true;
            onCurrentIndexChanged: {
                searchResource();
            }
        }

        MyTextField{
            id:inputSearch;
            width: 130;height: 20;
            placeholderText:qsTr("搜索")
            anchors.bottom: parent.bottom;
            anchors.right: typeSort.left;anchors.rightMargin: 15;
            text: ""
            font.pixelSize: 12;
            onTextChanged: {
                searchResource();
            }
        }
        TextButton{
            id: btnImportLocal
            text: qsTr("本地导入")
            anchors.bottom: parent.bottom;
            anchors.left: parent.left;//anchors.left: comboType.right;anchors.leftMargin: 20;
            size:large
            fontSize:14;
            mouseArea.onClicked: {
                fileDialog.onlyOpen = false;
                fileDialog.nameFilters = [qsTr("文件")+"("+nameFilter_game+")"];//+nameFilter_video+nameFilter_picture+")"];
                fileDialog.open();
            }
            visible: only_tools
        }

    }
    MyScrollView{
        anchors.top:titleBar.bottom;anchors.topMargin:20
        width: parent_width
        height:parent_height-titleBar.height-45;
        Rectangle{
        color: white_pure;
        width: parent_width;height:rectGrid.y+rectGrid.height;

        Rectangle{
            width:parent_width;height: parent_height-titleBar.height-1;
            anchors.top:titleBar.bottom;
            color: translucent
            visible: grid.count==0
            Image{
                id:imageNoResource;
                anchors.bottom: parent.verticalCenter;
                anchors.horizontalCenter: parent.horizontalCenter;
                source: "qrc:/resource/none.png";
            }

            MyLabel{
                id:labelNoResource;
                anchors.top: imageNoResource.bottom
                anchors.topMargin: 10;
                anchors.horizontalCenter: parent.horizontalCenter;
                text:qsTr("此处无内容")
            }
        }

        Rectangle{
            id:rectGrid;
            visible: grid.count>0;
            width:parent_width; height: grid.height;
            anchors.top:titleBar.bottom;anchors.topMargin:20
            color: translucent
            GridView{
                id:grid;
                width:countPerRow*gGRID_WIDTH;
                height: contentHeight+30;
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.top: parent.top;
                cellWidth: gGRID_WIDTH;cellHeight: gGRID_HEIGHT;
                model: resourceModel.searchResultList;
                objectName: "ResourceItem"
                visible: count>0;
                delegate: resourceDelegate
                focus:true
                clip: true;
            }

            Component{
                id:resourceDelegate;
                LibraryListItem{
                    resource:model.modelData;
                    width:grid.cellWidth-10; height:grid.cellHeight;
                    dialog:dialogType;
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
    }
    }
    function searchResource(){
        var keyWord = inputSearch.text;
        var resType = 1;//游戏//comboType.currentIndex;
        var sort = comboSort.currentIndex;
        if(dialogType==3){
            if(sort==0){
                sort = 3;
            }else if(sort==1){
                sort = 4;
            }
        }

        grid.model = 0;
        resourceModel.searchResource(keyWord,resType,sort,dialogType,typeSort.currentIndex);
        grid.model = resourceModel.searchResultList;
    }
}


/*
MyScrollView{
    visible: true;
    property int parent_width: parent.width;
    property int parent_height:parent.height
    property int dialogType: 0;//窗口类型,0:已下载，1：正在下载，2：待升级, 3:我的购买
    property bool show: false;
    property int gGRID_WIDTH: 275;
    property int gGRID_HEIGHT:220;
    property int comboBoxWidth:80;
    property int selectedIndex: 0;
    property int countPerRow: parseInt(width/gGRID_WIDTH);
    property int resCount: resourceModel.resCount;
    property int dlingCount: resourceModel.dlingCount;
    readonly property int dialog_bought: 3;

    Component.onCompleted: {
        searchResource();
    }
    onResCountChanged: {
        console.log("onResCountChanged,count="+resCount);
        searchResource();
    }
    onDlingCountChanged: {
        console.log("onDlingCountChanged,count="+dlingCount);
        searchResource();
    }
    onDialogTypeChanged: {
        inputSearch.text="";
        comboSort.currentIndex = 0;
        comboType.currentIndex = 0;
        searchResource();
    }
    Rectangle{
        color: white_pure;
        width: parent_width;height:rectGrid.y+rectGrid.height;
        Rectangle{//titlebar
            id:titleBar;
            width: 803//grid.width;
            anchors.top: parent.top;anchors.topMargin: 25;
            height: 30//50;
            anchors.horizontalCenter: parent.horizontalCenter;

            MyComboBox {
                id: comboType
                visible: false
                width:comboBoxWidth
                height: 20
                anchors.verticalCenter: parent.verticalCenter;
                anchors.left: parent.left;
                currentIndex: 0
                model: [qsTr("所有"),qsTr("游戏"),qsTr("视频"),qsTr("图片")]
                isBlack: true;
                onCurrentIndexChanged: {
                    searchResource();
                }
            }

            MyComboBox {
                id: comboSort
                borderColor: white
                height: 20
                width:comboBoxWidth+30;
                anchors.bottom: parent.bottom;
                anchors.right: parent.right;anchors.rightMargin: 15;
                currentIndex: 0
                model: dialogType==dialog_bought?[qsTr("购买时间"),qsTr("发布时间"),qsTr("名称")]:[qsTr("使用时间"),qsTr("导入时间"),qsTr("名称"),qsTr("游戏类型")]
                isBlack: true;
                onCurrentIndexChanged: {
                    searchResource();
                }
            }

            MyTextField{
                id:inputSearch;
                width: 130;height: 20;
                placeholderText:qsTr("搜索")
                anchors.bottom: parent.bottom;
                anchors.right: comboSort.left;anchors.rightMargin: 15;
                text: ""
                font.pixelSize: 12;
                onTextChanged: {
                    searchResource();
                }
            }
            TextButton{
                id: btnImportLocal
                text: qsTr("本地导入")
                anchors.bottom: parent.bottom;
                anchors.left: parent.left;//anchors.left: comboType.right;anchors.leftMargin: 20;
                size:large
                fontSize:14;
                mouseArea.onClicked: {
                    fileDialog.onlyOpen = false;
                    fileDialog.nameFilters = [qsTr("文件")+"("+nameFilter_game+")"];//+nameFilter_video+nameFilter_picture+")"];
                    fileDialog.open();
                }
                visible: only_tools
            }

        }

        Rectangle{
            width:parent_width;height: parent_height-titleBar.height-1;
            anchors.top:titleBar.bottom;
            color: translucent
            visible: grid.count==0
            Image{
                id:imageNoResource;
                anchors.bottom: parent.verticalCenter;
                anchors.horizontalCenter: parent.horizontalCenter;
                source: "qrc:/resource/none.png";
            }

            MyLabel{
                id:labelNoResource;
                anchors.top: imageNoResource.bottom
                anchors.topMargin: 10;
                anchors.horizontalCenter: parent.horizontalCenter;
                text:qsTr("此处无内容")
            }
        }

        Rectangle{
            id:rectGrid;
            visible: grid.count>0;
            width:parent_width; height: grid.height;
            anchors.top:titleBar.bottom;anchors.topMargin:20
            color: translucent
            GridView{
                id:grid;
                width:countPerRow*gGRID_WIDTH;
                height: contentHeight+30;
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.top: parent.top;
                cellWidth: gGRID_WIDTH;cellHeight: gGRID_HEIGHT;
                model: resourceModel.searchResultList;
                objectName: "ResourceItem"
                visible: count>0;
                delegate: resourceDelegate
                focus:true
                clip: true;
            }

            Component{
                id:resourceDelegate;
                LibraryListItem{
                    resource:model.modelData;
                    width:grid.cellWidth-10; height:grid.cellHeight;
                    dialog:dialogType;
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
    }

    function searchResource(){
        var keyWord = inputSearch.text;
        var resType = 1;//游戏//comboType.currentIndex;
        var sort = comboSort.currentIndex;
        if(dialogType==3){
            if(sort==0){
                sort = 3;
            }else if(sort==1){
                sort = 4;
            }
        }else{
            if(sort == 3){
                sort = 5;//for game type
            }
        }

        grid.model = 0;
        resourceModel.searchResource(keyWord,resType,sort,dialogType);
        grid.model = resourceModel.searchResultList;
    }
}
*/
