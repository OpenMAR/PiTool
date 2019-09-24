import QtQuick 2.7
import QtQuick.Controls 2.0

Text {
    property bool bold: false;
    font.pixelSize: 14
    font.family: bold?webFont_bold.name:webFont.name;
    font.bold: bold;
    color: black
    linkColor: blue;
    padding: 0;
    fontSizeMode:Text.VerticalFit;
}
