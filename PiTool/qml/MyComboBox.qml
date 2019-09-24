import QtQuick 2.7
import QtQuick.Controls 2.1

ComboBox {
    id: control
    property bool isBlack: true
    property var borderColor:blue
    background: Rectangle {
        color:translucent;
        border.color: control.focus?borderColor:white
        border.width: 1
        radius: rRADIUS;
    }
    contentItem: MyLabel {
        leftPadding: 0
        rightPadding: control.spacing
        text: control.displayText
        color: isBlack?black:white
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
    delegate: ItemDelegate {
        height: 35;
        width: control.width
        highlighted: control.highlightedIndex == index
        contentItem: MyLabel {
            text: modelData
            color: isBlack?black:white
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        background: Rectangle{
            color:isBlack?(highlighted?"#b7b7b7":white_pure):(highlighted?"#565656":translucent)
//            radius: 4;
        }
    }
    popup: Popup {
        y: control.height
        width: control.width
        implicitHeight: listview.contentHeight+2
        padding: 1

        contentItem: MyListView {
            id: listview
            clip: true
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex
            ScrollIndicator.vertical: ScrollIndicator { }
        }
        background: Rectangle {
            color: isBlack?white:black
            border.color: isBlack?translucent:"#565656"
            border.width: 1
            radius: rRADIUS;
        }
    }
    indicator: null;
}

