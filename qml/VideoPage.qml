import QtQuick 2.12
import QtQuick.Controls 2.5
import QtMultimedia 5.15

ApplicationWindow{
    id:videoDialog
    width : 800
    height : 600
    visible: false
    property alias video: video
    Video {
        id: video
        width : 800
        height : 600
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if(!videoPlayFlag) {
                    if(content.musicPlayer.pause.visible) {   //如果此时正在播放音乐，要暂停音乐播放
                        content.musicPlayer.pause.clicked()
                    }
                    videoDialog.video.play()
                    videoPlayFlag=true
                } else {
                    videoDialog.video.pause()
                    videoPlayFlag=false
                }
            }
        }
    }
}
