import QtQuick 2.7
import QtQuick.Controls 2.0
import pimaxvr.OnlineModel 1.0
import pimaxvr.CatalogItem 1.0
import pimaxvr.GameItem 1.0
import pimaxvr.FilterItem 1.0

Rectangle{
    id:main;
    color: "#313131";
    property string className: "OnlineGameSearch";
    property var stackView;
    property int cell_width:255;
    property int cell_height:180;
    property var gameList: null;
    property var gameRecommend: null;
    property bool bGame: true
    property var filterList:null;
    property string searchType:"allGame";//allGame,gameSearch,gameFilter;
    property string searchParam;
    property int totalItem:0;//总记录数
    property int currentPage:0;//当前页数
    property int totalPage:0;//每页的记录数
    property real gridView_StartY: 0;
    property string filterChecked:"";
    property string searchKeyWord:"";
    clip:true;

    Rectangle{
        width: resultPanel.width+resultPanel.x;height: parent.height
        color:"#313131"
        anchors.horizontalCenter: parent.horizontalCenter;
        Component.onDestruction: {
            console.log("onDestruction");
            if(gameList){
                gameList.clear();
                gameList = null;
            }
        }
        Component.onCompleted: {
            if(searchType=="gameFilter"||searchType=="videoFilter"){
                filterChecked = searchParam;
            }
            console.log("OnlineGameSearch onCompleted,search type&param",searchType,searchParam);
        }

        Rectangle{//
            id:searchPanel
            width:240;height:parent.height;
            color:cCOLOR_Translucent
            Rectangle{//search input
                id:layoutSearch
                color:cCOLOR_Translucent
                anchors.top: parent.top;anchors.topMargin: 20;
                width: parent.width;height: 40;
                radius: height/2;
                border{width:1;color:"#696969"}
                ImageButton{
                    id:btnSearch;
                    iconSource: 'qrc:/resource/search-n.png';
                    anchors.right: parent.right;anchors.rightMargin: parent.height/2;
                    anchors.verticalCenter: parent.verticalCenter;
                    mouseArea.onClicked: {
                        var keyword = inputSearch.text;
                        if(keyword==inputSearch.placeHolder){
                            keyword="";
                        }
                        searchGame(keyword,1);
                    }
                }
                TextInput{
                    id:inputSearch;
                    property var placeHolder: qsTr("请输入搜索内容")
                    anchors.verticalCenter: parent.verticalCenter;
                    anchors.right: btnSearch.left;anchors.rightMargin: 10;
                    anchors.left: parent.left;anchors.leftMargin: parent.height/2;
                    font.pixelSize: 14;
                    color:text==placeHolder?cCOLOR_GRAY:cCOLOR_WHITE;
                    clip: true
                    selectByMouse:true
                    text: searchType=="gameSearch"?searchParam:placeHolder
                    onFocusChanged: {
                        if(focus&&text==placeHolder){
                            text="";
                        }else if(!focus&&text==""){
                            text = qsTr("请输入搜索内容")
                        }
                    }
                    onAccepted: {
                        var keyword = inputSearch.text;
                        if(keyword==inputSearch.placeHolder){
                            keyword="";
                        }
                        searchGame(keyword,1);
                    }
                }
            }

            MyListView{
                id:filterListView;
                width: parent.width;
                anchors.top: layoutSearch.bottom;anchors.topMargin: 30;
                height: main.height-y;
                orientation: ListView.Vertical;
                spacing: 0;
                objectName: "FilterItem"
                model: onlineModel.gameFilterList();
                delegate: typeListDelegate;
                onModelChanged: {
                    console.log("onlineModel.gameFilters,",model);
                }
            }
            Component{
                id:typeListDelegate;
                OnlineFilterItem{
                    filtersItem:model.modelData;
                    checkbox.checked: filtersItem==null?false:filterChecked.indexOf(""+filtersItem.id)>=0;
                    checkbox.onClicked: {
                        if(checkbox.checked){
                            if(filterChecked==""){
                                filterChecked+=filtersItem.id;
                            }else{
                                filterChecked+=","+filtersItem.id;
                            }
                        }else{
                            filterChecked = filterChecked.replace(","+filtersItem.id,"");
                            filterChecked = filterChecked.replace(filtersItem.id+",","");
                            filterChecked = filterChecked.replace(filtersItem.id+"","");
                        }
                        console.log("OnlineGameSearch MyCheckBox onClicked,",filterChecked);
                        filterGame(filterChecked,1);
                    }
                }
            }
        }
        Rectangle{
            id:resultPanel;
            anchors.top: parent.top;
            anchors.left: searchPanel.right;anchors.leftMargin: 50;
            width:{
                var nWidth = main.width-searchPanel.width-100;
                nWidth = parseInt(nWidth/cell_width)*cell_width;
                var width = ((nWidth>cell_width)?nWidth:cell_width)+20;
                console.log("resultPanel,",nWidth,width);
                return width;
            }
            height: parent.height
            color:cCOLOR_Translucent
            Rectangle{
                id:layoutTitle
                color:cCOLOR_Translucent
                anchors.top: parent.top;
                width:parent.width;height: title.height;
                TitleLabel{
                    id:title
                    text: {
                        var txt = bGame?qsTr("全部游戏"):qsTr("全部视频");
                        if(searchType=="gameSearch" || searchType=="videoSearch"){
                            txt = qsTr("搜索结果");
                        }else if(searchType=="gameFilter"||searchType=="videoFilter"){
                            txt = qsTr("检索结果")
                        }
                        txt = txt + qsTr("(共")+totalItem+qsTr("项)");
                        return txt;
                     }
                }
            }
            Rectangle{
                id:panelNoResource;
                width: parent.width;
                height: parent.height-layoutTitle.height-20;
                anchors.top: layoutTitle.bottom;anchors.topMargin: 20;
                visible: gameList==null||gameList.count==0
                color: cCOLOR_Translucent
                Rectangle{
                    id:textNoResource;
                    color:cCOLOR_BLACK_DEEP;
                    width: parent.width;
                    height: labelNoResource.height+80;
                    MyLabel{
                        id:labelNoResource;
                        text:qsTr("老铁，根据您的搜索条件，共找到0个结果<br/>可以去<a href='http://bbs.pimaxvr.com'>小派论坛</a>看看哦~")
                        linkColor: color;
                        horizontalAlignment:Text.AlignHCenter;
                        anchors.centerIn: parent;
                        wrapMode:Text.WordWrap;
                        padding:5
                        lineHeight: 2
                        onLinkActivated: {
                            Qt.openUrlExternally(link);
                        }
                    }
                }
                MyLabel{
                    id:recommendTitle;
                    text:qsTr("------------为你推荐------------")
                    anchors.horizontalCenter: parent.horizontalCenter;
                    anchors.top: textNoResource.bottom;anchors.topMargin: 40;
                    visible: gameRecommend&&(gameRecommend.count>0)
                }
                MyListView{
                    id:recommendList
                    height: 180;width: parent.width;
                    anchors.top: recommendTitle.bottom;anchors.topMargin: 15;
                    anchors.horizontalCenter: parent.horizontalCenter;
                    orientation: ListView.Horizontal;
                    visible: gameRecommend&&(gameRecommend.count>0)
                    spacing: 15;
                    delegate:recommendDelegate;
                    model: gameRecommend?gameRecommend.count:0
                }
                Component{
                    id:recommendDelegate;
                    Rectangle{
                        height:item.height;width: item.width
                        color:cCOLOR_Translucent
                        OnlineGameItem{
                            id:item
                            gameItem: gameRecommend.get(index);
                            mouseArea.onClicked: {
                                if(bGame){
                                    showGameDetail(gameRecommend.get(index).id);
                                }else{
                                    showVideoDetail(gameRecommend.get(index).id);
                                }
                            }
                        }
                    }
                }
            }

            MyScrollView{
                id:scrollView;
                width: resultPanel.width;
                height: resultPanel.height-layoutTitle.height-20;
                anchors.top: layoutTitle.bottom;anchors.topMargin: 20;
                anchors.horizontalCenter: parent.horizontalCenter;
                GridView{
                    id:gridView;
                    width: resultPanel.width;
                    height: contentHeight;
//                    anchors.top: layoutTitle.bottom;anchors.topMargin: 20;
                    cellWidth: cell_width;cellHeight: cell_height;
//                    anchors.horizontalCenter: parent.horizontalCenter;
                    model:gameList==null?0:gameList.count
                    visible: gameList!=null&&gameList.count>0
                    delegate: gameDelegate;
                    focus:true
                    clip: true;
                    onModelChanged: {
//                        console.log("GridView.onModelChanged,",atYEnd,contentY,model);
                        if(model>0){
                            contentY = gridView_StartY;
                        }
                    }

//                    onMovementEnded:{
//                        console.log("onMovementEnded,",atYEnd,contentY,currentPage,totalPage,totalItem);
//                        gridView_StartY = contentY;
//                        if(atYEnd && currentPage*totalPage<totalItem){
//                            if(searchType=="allGame"||searchType=="allVideo"){
//                                getAllGame(currentPage+1)
//                            }else if(searchType=="gameSearch"||searchType=="videoSearch"){
//                                searchGame(searchParam,currentPage+1);
//                            }else if(searchType=="gameFilter"||searchType=="videoFilter"){
//                                filterGame(searchParam,currentPage+1);
//                            }
//                        }
//                    }
                    footer:Rectangle{
                        width: resultPanel.width;
                        height: currentPage*totalPage>=totalItem?60:0;
                        color: cCOLOR_Translucent;
                        visible:currentPage*totalPage>=totalItem
                        MyLabel{
                            anchors.centerIn: parent
                            text:qsTr("------------我是有底线的------------");
                        }
                    }
                }
            }
            Component{
                id:gameDelegate;
                OnlineGameItem{
                    gameItem: gameList.get(index);
                    width:cell_width-15;height: cell_height-10;
                    mouseArea.onClicked: {
                        if(bGame){
                            showGameDetail(gameList.get(index).id);
                        }else{
                            showVideoDetail(gameList.get(index).id);
                        }
                    }
                }
            }
        }
    }

    function searchGame(keyword,page){
        console.log("searchGame,",keyword,page);
        if(bGame){
            onlineModel.searchGame(keyword,page,"score",true);
        }else{
            onlineModel.searchVideo(keyword,page,"score",true);
        }
    }

    function filterGame(filter,page){
        console.log("filterGame,",filter,page);
        if(bGame){
            onlineModel.filterGame(filter,page,"score",true);
        }else{
            onlineModel.filterVideo(filter,page,"score",true);
        }
    }

    function getAllGame(page){
        if(bGame){
            onlineModel.getAllGame(page, searchParam, true);
        }else{
            onlineModel.getAllVideo(page, searchParam, true);
        }
    }
}
