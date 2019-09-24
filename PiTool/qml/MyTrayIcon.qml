import QtQuick 2.7
import QtQuick.Controls 2.0
import Qt.labs.platform 1.0

    SystemTrayIcon {
        id:trayIcon;
        iconSource: "qrc:/resource/tray.png"
        tooltip:connect?"PiTool":qsTr("小派VR设备未连接");
        onActivated: {
            console.log("trayIcon onActivated ",reason);
            if(reason == SystemTrayIcon.DoubleClick){
                showWindow();
            }
        }

        menu: Menu {
            font:Qt.font({family:webFont.name,pointSize:10})
            MenuItem {
                text:qsTr("启动SteamVR");
                onTriggered: {
                    if(resourceModel.steamVrIsInstalled()){
                        resourceModel.launchSteamVr();
                    }else{
                        showInstallSteamVr("");
                    }
                }
            }
            MenuItem {
                text:qsTr("方向校准");
                onTriggered: {
                    if(piService.resetEyePose()){
                        showAlertPopup2(qsTr("方向重置成功!"));
                    }else{
                        showAlertPopup2(qsTr("方向重置失败!"));
                    }
                }
            }
            MenuItem {
                text:qsTr("打开PiTool");
                onTriggered: {
                    showWindow();
                }
            }
            MenuItem {
                text:qsTr("退出");
                iconName: "edit-undo"
                iconSource:"qrc:/resource/trayicon-close.png"
                onTriggered: {
                    Qt.quit();
                }
            }
        }
        onMessageClicked: showWindow();
        MyMouseArea{
            id:mouseArea
            onEntered: {
                console.log("trayIcon onEntered ");
            }
            onExited: {
                console.log("trayIcon onExited ");
            }
        }
    }

