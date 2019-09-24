import QtQuick 2.1
import QtQuick.Window 2.2
import QtWebEngine 1.4

Window {
    id:browserDialog
    property alias currentWebView: webView
    flags: Qt.Dialog
    width: 800
    height: 600
    visible: true
    onClosing: destroy()
    WebEngineView {
        id: webView
        anchors.fill: parent
        Component.onCompleted: {
        }
        onContentsSizeChanged: {
            console.log("webview size",contentsSize.width,contentsSize.height);
        }
        onWindowCloseRequested: {
            console.log("onWindowCloseRequested");
            browserDialog.close();
        }
        onNewViewRequested: {
            if (!request.userInitiated)
                print("Warning: Blocked a popup window.")
            else if (request.destination == WebEngineView.NewViewInTab) {
//                var tab = tabs.createEmptyTab(currentWebView.profile)
//                tabs.currentIndex = tabs.count - 1
//                request.openIn(tab.item)
            } else if (request.destination == WebEngineView.NewViewInBackgroundTab) {
//                var tab = tabs.createEmptyTab(currentWebView.profile)
//                request.openIn(tab.item)
            } else if (request.destination == WebEngineView.NewViewInDialog) {
                var dialog = createBrowserDialog(webView.profile);
                request.openIn(dialog.currentWebView)
            } else {
//                var window = applicationRoot.createWindow(currentWebView.profile)
//                request.openIn(window.currentWebView)
            }
        }
    }
}
