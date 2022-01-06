import QtQuick 2.12
import QtQuick.Controls 2.5
import QtMultimedia 5.15
import QtQuick.Dialogs 1.3
ApplicationWindow{
    id:videoDialog
    visible: false
    width : 850
    height : 560
    property alias video: video
    title:kugou.kuGouMv.mvName[mvListView.currentIndex]===undefined?"":kugou.kuGouMv.mvName[mvListView.currentIndex].replace('<em>','').replace('</em>','')
    Video {
        id: video
        anchors.fill: parent
        fillMode: VideoOutput.Stretch
        MouseArea {
            id:mouseArea5
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
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

    onClosing: {
        video.stop()
    }
    TapHandler{
        id:tapHandler
        acceptedButtons: Qt.RightButton
        onTapped: {
            mX=mouseArea5.mouseX
            mY=mouseArea5.mouseY
            menu3.open()
        }
    }
    Menu{
        id:menu3
        x:mX
        y:mY
        contentData: [
            downloadMv,
        ]
   }
   Action{
       id:downloadMv
       text: qsTr("保存到本地")
       onTriggered: {
           saveMvDialog.open()
       }
   }
   FileDialog{
       id: saveMvDialog
       title: "save mv"
       folder:shortcuts.documents
       nameFilters: ["*.mp4"]
       selectExisting: false
       onAccepted: {
           var path=fileUrl.toString().slice(7)
           var end=path.substring(path.length-4)
           if(end!=="mp4") {
               path+=".mp4"
           }
           kugou.kuGouMv.downloadMv(mvListView.currentIndex,path)
       }
   }
}
