import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

ScrollView{
    horizontalScrollBarPolicy:Qt.ScrollBarAlwaysOff;
    property int scrollbar_width: 10
    style: ScrollViewStyle{
        scrollBarBackground: Rectangle {
            implicitWidth: scrollbar_width
            implicitHeight: 30
            color: translucent
        }
        decrementControl: null
        incrementControl: null
        handle: Rectangle {
            implicitWidth: scrollbar_width
            implicitHeight: 30
            color: white
            radius: scrollbar_width/2;
        }
    }
}
