import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtWebEngine 1.4

MyWebView{
    id:webview;
    onNewViewRequested: {
        console.log("onNewViewRequested,",request.destination,url);
//        if (!request.userInitiated)
//            print("Warning: Blocked a popup window.")
//        else
        if (request.destination == WebEngineView.NewViewInTab) {
            var loginDialog = showLoginDialog(null,webview.profile,webview);
            request.openIn(loginDialog.currentWebView)
        } else if (request.destination == WebEngineView.NewViewInBackgroundTab) {
        } else if (request.destination == WebEngineView.NewViewInDialog) {
            var browserDialog = createBrowserDialog(webview.profile);
            request.openIn(browserDialog.currentWebView)
        } else {
        }
    }
    onUrlChanged: {
        console.log("OnlineBrowser",url);
        if(url.toString().indexOf("login")>0){
            showLoginDialog();
            return;
        }
        webview.runJavaScript("window.localStorage.getItem('piplay_token')",function(result){
            console.log("runJavaScript,piplay_token=",result);
            if(result){
                access_token = result;
                isLogin = true;
            }else{
                access_token = "";
                isLogin = false;
            }
        });
        webview.runJavaScript("window.localStorage.getItem('nick_name')",function(result){
            console.log("runJavaScript,user=",result);
            if(result){
                nick_name = result;
            }else{
                nick_name = "";
            }
        });
    }
    onLoadProgressChanged:{

    }
    settings.autoLoadImages: true;
}

