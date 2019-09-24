import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

TextInput {
    property var elide: null;
    property double lineHeight: 1.1
    property var onLinkActivated;
    property var linkColor;
    property bool bold: false
    selectByMouse: true;
    readOnly: true;
    font.pixelSize: 14
    font.wordSpacing:0;
    color: black
    font.family: bold?webFont_bold.name:webFont.name;
    linkColor: blue;
    clip: true;
    autoScroll: false;
    onSelectedTextChanged: {
        autoScroll=true;
    }
    selectionColor:"#0078D7"
    selectedTextColor:black
}

