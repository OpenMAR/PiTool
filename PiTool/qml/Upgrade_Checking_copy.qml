import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

Window {
    id:connectHelpDialog
    flags: Qt.Popup;
    modality:Qt.WindowModal
    color:"#B2000000"
    property int margin: 20;
    Rectangle{
        width:800;
        height: 600
        color:"#222222"
        radius: 5;
        anchors.centerIn: parent;
        Image{
            id:image_bg;
            source: "qrc:/resource/pc.png";
            anchors.centerIn: parent;
        }
        ImageButton {//Close Button
            id: btnClose
            anchors.right: parent.right;anchors.rightMargin: 10;
            anchors.top: parent.top;anchors.topMargin: 10
            image_normal: "qrc:/resource/quit-n.png"
            image_hover: "qrc:/resource/quit-h.png"
            image_pressed: "qrc:/resource/quit-c.png"
            mouseArea.onClicked: {
                connectHelpDialog.close();
            }
        }
        Image{
            id:usbPosition;
            source:piService.usbState?"qrc:/resource/line_pc_usb_n.png":"qrc:/resource/line_pc_usb_h.png";
            x:270;
            y:300;
        }

        Image{
            id:hdmiPosition;
            source:piService.hdmiState?"qrc:/resource/line_pc_hdmi_n.png":"qrc:/resource/line_pc_hdmi_h.png";
            x:420;
            y:317;
        }

        ConnectStatusUsb{
            id:rectUsb;
            anchors.top: usbPosition.top;anchors.topMargin: -height/2;
            anchors.right: usbPosition.left;
        }
        ConnectStatusHdmi{
            id:rectHdmi;
            anchors.top: hdmiPosition.top;anchors.topMargin: -height/2;
            anchors.left: hdmiPosition.right;
        }
    }

}
