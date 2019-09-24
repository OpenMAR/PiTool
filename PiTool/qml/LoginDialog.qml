import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtWebEngine 1.4

MyDialog {
    property alias currentWebView: webview
    property WebEngineView from: null;
    id:loginDialog
    onClosing: destroy();
    clientWidth:400;
    clientHeight: 285;
    title: qsTr("登录")
    property var callback: null;
    MyWebView{
        id:webview;
        parent:clientArea;
        width:parent.width;height: parent.height;
        url:base_url+"/login?"+settingModel.getLangAndLcid()
        onUrlChanged: {
            console.log("LoginDialog",url);
            if(url.toString().indexOf("register")>0){
                clientHeight = 385;
                loginDialog.title = qsTr("注册");
            }else if(url.toString().indexOf("resetPwd")>0){
                clientHeight = 330;
                loginDialog.title = qsTr("重置密码");
            }else{
                clientHeight = 285;
                loginDialog.title = qsTr("登录");
            }
            var time=0;
            webview.runJavaScript("window.localStorage.getItem('piplay_token')",function(result){
                console.log("runJavaScript,piplay_token=",result);
                if(result){
                    access_token = result;
                    isLogin = true;
                }else{
                    access_token = "";
                    isLogin = false;
                }
                time++;
                if((url.toString().indexOf("mine/myProfile")>0
                        ||url.toString().indexOf("home")>0)&&time==2){
                    loginDialog.close();
                    if(callback){
                        callback();
                    }
                }
            });
            webview.runJavaScript("window.localStorage.getItem('nick_name')",function(result){
                console.log("runJavaScript,user=",result);
                if(result){
                    nick_name = result;
                }else{
                    nick_name = "";
                }
                time++;
                if((url.toString().indexOf("mine/myProfile")>0
                        ||url.toString().indexOf("home")>0)&&time==2){
                    loginDialog.close();
                    if(callback){
                        callback();
                    }
                }
            });
        }
        onNewViewRequested: {
            console.log("loginDialog, onNewViewRequested,",request.destination,url);
            if (request.destination == WebEngineView.NewViewInTab) {
                if(from){
                    from.profile = webview.profile;
                    request.openIn(from);
                }
            } else if (request.destination == WebEngineView.NewViewInBackgroundTab) {
            } else if (request.destination == WebEngineView.NewViewInDialog) {
            } else {
            }
            loginDialog.close();
        }
        onWindowCloseRequested: {
            loginDialog.close();
        }
    }
}
