import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.2

ApplicationWindow{
    id:root
    width: 700
    height: 200
    flags: Qt.FramelessWindowHint
    color: Qt.rgba(0,0,0,0)

    property alias musicStart: musicStart
    property alias musicPause:musicPause
    property alias miniText: miniText

    MouseArea{
        anchors.fill: parent
        property point clickPos: "0,0"
        onPressed: {
            clickPos = Qt.point(mouse.x, mouse.y)
        }
        onPositionChanged: {
            var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
            root.x = root.x+delta.x
            root.y = root.y+delta.y
        }
    }

    HoverHandler{
        onHoveredChanged: {
            if(hovered){
                toolRec.opacity =1
                toolRec.visible=true
                root.color = Qt.rgba(0.5,0.5,0.5,0)
            }
            if(!hovered){
                toolRec.opacity =0
                root.color = Qt.rgba(0,0,0,0)
                toolRec.visible=false
            }
        }
    }
    ColumnLayout{
        anchors.fill: parent
        RowLayout {
            id: toolRec
            height: 30
            visible: false
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            ToolButton {
                id:musicPrevious
                icon.source: "qrc:/image/上一曲.png"
                onClicked: {
                    actions.previousAction.triggered()
                }
            }
            ToolButton{
                id:musicStart
                icon.source:"qrc:/image/播放.png"
                visible: true
                onClicked: {
                    actions.playAction.triggered()
                }
            }
            ToolButton{
                id:musicPause
                visible: false
                icon.source:"qrc:/image/暂停.png"
                onClicked: {
                    actions.pauseAction.triggered()
                }
            }

            ToolButton {
                id:musicNext
               icon.source:"qrc:/image/下一曲.png"
                onClicked: {
                    actions.nextAction.triggered()
                }
            }
            ToolButton {
                id:musicFastfowardfivescd
                icon.source:"qrc:/image/快退.png"
                onClicked: {
                    actions.backfiveScdAction.triggered()
                }
            }
            ToolButton {
                id:musicBackfivescd
                icon.source:"qrc:/image/快进.png"
                onClicked: {
                    actions.fastforwardfiveScdAction.triggered()
                }
            }
            ToolButton {
                icon.source:"qrc:/image/close.png"
                onClicked: {
                    close()
                    content.musicPlayer.wordBackground.visible = false
                    content.musicPlayer.word.visible = true
                    content.musicPlayer.wordFlag=false
                }
            }
        }
        Text {
            id: miniText
            Layout.fillHeight: true
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
            opacity: 1
            color: "red"
            font.pixelSize:30
            text: " "
            MouseArea{
                anchors.fill: parent
                property point clickPos: "0,0"
                onPressed: {
                    clickPos = Qt.point(mouse.x, mouse.y)
                }
                onPositionChanged: {
                    var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
                    root.x = root.x+delta.x
                    root.y = root.y+delta.y
                }
            }
        }
    }
}
