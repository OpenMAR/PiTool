import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import pimaxvr.PiService 1.0
import QtQuick.Dialogs 1.2

MyScrollView{
    property int parent_width: parent.width
    width: parent.width;height: parent.height;
    //anchors.centerIn: parent;
    MyLabel{
        id:privacy;
        width: parent_width-60;height: contentHeight+100;
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
