import QtQuick 2.7
import pimaxvr.PiService 1.0
import QtQuick.Controls 2.1
import QtWebEngine 1.4
import QtQuick.Controls 1.4 as Control14
WebEngineView{
    id:webview;
    BusyIndicator{
        id:busyIndicator;
        anchors.centerIn: parent;
        running: webview.loadProgress<100
    }
    onUrlChanged: {

    }
    onLoadProgressChanged: {
        console.log("onLoadProgressChanged,",loadProgress);
    }
}

