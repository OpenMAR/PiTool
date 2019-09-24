import QtQuick 2.7
import QtQuick.Controls 2.1

MouseArea{
    id:mouseArea
    anchors.fill: parent;
    hoverEnabled: true;
    cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor
}
