import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import pimaxvr.Setting 1.0

MyDialog2 {
    id:checkingDialog
    property int mMargin: 30;
    property int leftMargin: 40;
    property int topMargin: 40;
    property int rightMargin: 40;
    property int fontSize: 18;
    property var time: 10000;
    property var session: ""
    property var errorMsg: "";
    property bool activeSuccess: false;
    property var product: null;

    title:qsTr("激活产品");
    clientWidth: 400;
    onClosing: destroy();
    Component.onCompleted: {
        var myDate = new Date();
        time =myDate.getTime();
        var random = Math.random()*10000;
        session = ""+random+time;
    }

    Rectangle{//激活界面
        parent:clientArea;
        width: parent.width;height: parent.height;
        color:translucent
        visible: !activeSuccess
        MyLabel{
            id:labelErrorMsg;
            text:errorMsg;
            anchors.top: parent.top;anchors.topMargin: 20;
            anchors.horizontalCenter: parent.horizontalCenter;
            color: "#FF0000";
        }

        MyLabel{
            id:labelExchangeCode;
            text: qsTr("请输入您的激活码：")
            anchors.top: labelErrorMsg.bottom;anchors.topMargin: 10;
            anchors.left: parent.left;anchors.leftMargin: 40;
        }
        MyTextField{
            id:inputExchangeCode
            anchors.verticalCenter:labelExchangeCode.verticalCenter
            anchors.left: labelExchangeCode.right;anchors.leftMargin: 10;
            anchors.right: parent.right;anchors.rightMargin: rightMargin;
        }
        MyLabel{
            id:labelVerifyCode;
            text: qsTr("请输入验证码：")
            anchors.top: labelExchangeCode.bottom;anchors.topMargin: mMargin;
            anchors.right: labelExchangeCode.right;

        }
        MyTextField{
            id:inputVerifyCode
            anchors.verticalCenter:labelVerifyCode.verticalCenter
            anchors.left: inputExchangeCode.left;
            anchors.right: image.left;anchors.rightMargin: 10;
        }
        ImageButton{
            id:refresh;
            anchors.verticalCenter:labelVerifyCode.verticalCenter
            anchors.right: parent.right;anchors.rightMargin: rightMargin;
            image_normal:"qrc:/resource/refresh-black.png"
            mouseArea.onClicked: {
                var myDate = new Date();
                time =myDate.getTime();
            }
        }

        Image{
            id:image
            width: 80;height: 24;
            anchors.verticalCenter:labelVerifyCode.verticalCenter
            anchors.right: refresh.left;anchors.rightMargin: 10;
            source: session==""?"":base_http+"/user/verifycode?session="+session+"&t="+time;
        }

        TextButton{
            text: qsTr("立即激活")
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.bottom: parent.bottom;anchors.bottomMargin: mMargin;
            size:large;
            mouseArea.onClicked:{
                exchangeProduct(inputExchangeCode.text,inputVerifyCode.text);
            }
        }
    }

    Rectangle{//激活成功
        parent:clientArea;
        width: parent.width;height: parent.height;
        color:translucent
        visible: activeSuccess

        MyLabel{
            text: product?'"'+product.title+'"'+qsTr("激活成功!"):""
            anchors.bottom:parent.verticalCenter;anchors.bottomMargin: 20;
            anchors.horizontalCenter: parent.horizontalCenter
        }

        TextButton{
            text: qsTr("确定")
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: parent.verticalCenter;anchors.topMargin: 20;
            size:large;
            mouseArea.onClicked:{
                checkingDialog.close();
            }
        }
    }
    function exchangeProduct(exchangeCode,checkcode){
        if(!exchangeCode){
            errorMsg = qsTr("请输入激活码");
            return;
        }
        if(!checkcode){
            errorMsg = qsTr("请输入验证码");
            return;
        }
        errorMsg="";

        var request = new XMLHttpRequest();
        request.onreadystatechange=function() {
            if(request.readyState == XMLHttpRequest.DONE) {
                if (request.status == 200) {
                    console.log("Response = " + request.responseText);
                    var response = JSON.parse(request.responseText);
                    if(response.code==8888){
                        activeSuccess = true;
                        product = response.data;
                        resourceModel.downloadResouce(product.id,"",product.type,product.subType,product.url,product.image_url,product.title,product.desc);
                    }else{
                        errorMsg = response.message;
                    }
                }
                else {
                    console.log("Status: " + request.status + ", Status Text: " + request.statusText);
                }
            }
        }
        var url = base_http+"/exchangecode/active?session="+session+"&checkcode="+checkcode
                +"&code="+exchangeCode+"&access_token="+access_token+"&"+settingModel.getLangAndLcid();
        console.log("url="+url);
        request.open("get",url, true);
        request.send();
    }
}
