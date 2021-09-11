import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQml.Models 2.12
import QtQuick.Layouts 1.0

ApplicationWindow {
    id: root
    width: 400
    height: 390
    title: qsTr("最近播放")
    property real mX: 0.0
    property real mY: 0.0
    color: "white"

    property alias recentlyListModel: recentlyListModel
    property int rightIndex: 0
    property var recUrls: []
    function getMusicName(path){
        var fileName
        for(var i=path.length-1;i>=0;i--) {
            if(path[i]==="."){
                path=path.slice(0, i)
            }
            if(path[i]==="/") {
                fileName=path.slice(i+1)
                break;
            }
        }
        return fileName
    }

    Action{
        id: playRecently
        text: qsTr("播放")
        shortcut: "audio-play"
        icon.source:  "qrc:/image/播放.png"
        onTriggered: {
            dialogs.lyricDialog.fileIo.readUrls(dialogs.recentlyPlayDialog.rightIndex, "/tmp/KirinMusic/最近播放.txt")
            content.musicPlayer.audio.source = dialogs.lyricDialog.fileIo.source
            content.musicPlayer.setMusicName(dialogs.lyricDialog.fileIo.source)

            content.spectrogram.getVertices()
            content.spectrogram.speTimer.running = true
            dialogs.recentlyPlayDialog.close()

            actions.playAction.triggered()
        }
    }

    Action{
        id: delRecently
        text: qsTr("删除")
        icon.source:  "qrc:/image/delete.png"
        onTriggered: {
            dialogs.lyricDialog.fileIo.deleteUrls(dialogs.recentlyPlayedDialog.rightIndex, "/tmp/KirinMusic/最近播放.txt")
            dialogs.recentlyPlayedDialog.recentlyListModel.remove(dialogs.recentlyPlayedDialog.rightIndex, 1)
        }
    }

    ColumnLayout{
        anchors.fill: parent
        RowLayout{
            id: row
            z: 1
            spacing: 0
            Layout.leftMargin: 5
            Text {
                Layout.preferredWidth: 50
                Layout.preferredHeight: 30
                text: qsTr("序号")
                font.pixelSize: 15
            }
            Text {
                Layout.preferredWidth: 180
                Layout.preferredHeight: 30
                text: qsTr("歌曲")
                font.pixelSize: 15
            }
            Text {
                Layout.preferredWidth: 165
                Layout.preferredHeight: 30
                text: qsTr("歌手")
                font.pixelSize: 15
            }
        }

        ListView{
            id: recentlyListView
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height-110
            model: recentlyListModel
            delegate: recentlyDelegate
        }
        Button{
            id:empty
            text: qsTr("清空")
            Layout.rightMargin: 8
            Layout.alignment: Qt.AlignRight | Qt.AlignBottom
            onClicked: {
                dialogs.lyricDialog.fileIo.deleteAllUrls("/tmp/KirinMusic/最近播放.txt")
                recentlyListModel.clear()
            }
        }
    }

    ListModel{
        id: recentlyListModel
   }
   Component{
       id: recentlyDelegate
       Rectangle {
           width: 400
           height: 30
           color: ListView.isCurrentItem?"#E0F2F7":"white"
           RowLayout{
               width: parent.width
               anchors.left: parent.left
               anchors.leftMargin: 5
               spacing: 5
               Text {
                   id: serialnumber
                   text: index + 1
                   font.pixelSize: 15
                   Layout.preferredWidth: 45
               }
               Text {
                   id: songname
                   text: songName
                   font.pixelSize: 15
                   Layout.preferredWidth: 175
                   elide: Text.ElideRight   //省略显示文本
               }
               Text {
                   id: singername
                   text: singer
                   font.pixelSize: 15
                   Layout.preferredWidth: 160
                   elide: Text.ElideRight
               }
           }
           focus: true
           MouseArea{
               id:mouseArea
               acceptedButtons: Qt.RightButton|Qt.LeftButton   //点击右键，content 响应右键的上下文菜单
               anchors.fill: parent
               onClicked: {
                   if(mouse.button==Qt.RightButton) {
                       mX=mouseX
                       mY=mouseY
                       recentlyMenu.open()
                       rightIndex = index
                   }else{
                       recentlyMenu.close()
                       recentlyListView.currentIndex = index
                   }
               }
           }
           Menu{
               id:recentlyMenu
               x:mX
               y:mY
               contentData:[
                    playRecently,
                    delRecently
               ]
           }
       }
   }
   onClosing: {
       if(dialogs.recentlyPlayDialog.recUrls.length !== 0){
           dialogs.lyricDialog.fileIo.saveRecentlyUrls(dialogs.recentlyPlayDialog.recUrls)
       }
   }
}
