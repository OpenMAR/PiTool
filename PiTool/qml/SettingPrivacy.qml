import QtQuick 2.7
import QtQuick.Controls 2.0

MyScrollView{
    property int parent_width: parent.width
    width: parent.width;height: parent.height;
    anchors.centerIn: parent;
    MyLabel{
        id:privacy;
        width: parent_width;height: contentHeight+100;
        topPadding: 40;bottomPadding: 40;
        leftPadding: 40;rightPadding: 20;
        text:settingModel.privacy;
        wrapMode:Text.WordWrap;
        lineHeight: 1.2
        clip: true;
        textFormat: Text.RichText;
        linkColor: "#FFDE00"
        onLinkActivated: {
            Qt.openUrlExternally(link);
        }
    }
}
