import QtQuick 2.15
import QtQuick.Controls 1.4 as QQ
import QtQuick.Controls 2.15 as QQC
QQC.ApplicationWindow{
    property alias netLyric:netLyric
    id:showLyric
    width: 400
    height: 500
    property double mX:0.0
    property double mY:0.0
    title: qsTr("歌词预览")
    QQ.TextArea{
        id:netLyric
        anchors.fill: parent
        readOnly: true
        MouseArea{
            id:mouseArea
            acceptedButtons: Qt.RightButton
            anchors.fill: parent
            onClicked: {
                mX=mouseX
                mY=mouseY
                menu2.open()
            }
        }
        QQC.Menu{
            id:menu2
            x:mX
            y:mY
            contentData: [
                copy,
            ]
        }
    }

    QQC.Action{
        id:copy
        text:qsTr("复制")
        icon.name: "edit-copy"
        onTriggered: {
            netLyric.copy()
        }
    }
    onClosing: {
        netLyric.text=""
    }
}
