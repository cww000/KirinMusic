import QtQuick 2.12
import QtQuick.Dialogs 1.3 as QQ
import Qt.labs.platform 1.1
Item {
    function openMusicFileDialog(){
        fileMusicDialog.open();
    }

    function openMusicFolderDialog(){
        folderMusicDialog.open();
    }
    function openAboutDialog(){
        aboutDialog.open()
    }

    property alias fileMusicDialog:fileMusicDialog
    property alias folderMusicDialog:folderMusicDialog
    property alias fileImageDialog: fileImageDialog
    property alias songTagDialog:songTagDialog
    property alias lyricDialog:lyricDialog
    property alias recentlyPlayDialog: recentlyPlayDialog
    property alias keyMapDialog:keyMapDialog
    property alias lyricSearchDialog:lyricSearchDialog
    property alias songSearchDialog:songSearchDialog
    property alias miniDialog: miniDialog
    property alias saveDialog: saveDialog
    property alias skinDialog: skinDialog


    QQ.FileDialog{
        id:fileMusicDialog
        title: "请选择音乐文件"
        folder:shortcuts.documents
        nameFilters: ["Audio Files(*.mp3 *.ogg *.flac *wav)"]
        selectMultiple: true
    }

    FolderDialog{
        id:folderMusicDialog
        title:"请选择一个文件夹"
    }
    QQ.FileDialog{
        id: fileImageDialog
        title: "请选择一张图片"
        folder: shortcuts.documents
        nameFilters: [ "Image files (*.png *.jpeg *.jpg)" ]
        onAccepted: {
            imageUrl = fileImageDialog.fileUrl
            dialogs.songTagDialog.showImage()
            dialogs.lyricDialog.fileIo.saveBackgroundUrl(imageUrl)
            dialogs.skinDialog.usingImage = imageUrl
            dialogs.skinDialog.close()
        }
    }
    QQ.MessageDialog{
        id:aboutDialog
        title:"关于"
        text:"本软件名为麒麟音乐，由程炆炆，李纯林，聂海艳共同创造\n麒麟音乐可让你在享受音乐世界的同时体验编辑歌词，编辑歌曲信息的乐趣"
        standardButtons: StandardButton.Ok
    }

    SongTagDialog{
        id:songTagDialog
        visible: false
    }
    LyricDialog{
        id:lyricDialog
        visible: false
    }
    RecentlyPlayDialog{
        id: recentlyPlayDialog
        visible: false
    }
    KeyMapDialog{
        id:keyMapDialog
        visible: false
    }
    LyricSearchDialog{
        id:lyricSearchDialog
        visible: false
    }
    SongSearchDialog{
        id:songSearchDialog
        visible: false
    }
    MiniDialog{
        id:miniDialog
        visible: false
    }
    SKinDialog{
        id:skinDialog
        visible: false
    }

    QQ.MessageDialog{
        id: saveDialog
        title: qsTr("提示")
        text: qsTr("曲目信息已被修改，是否想要保存?")
        standardButtons: StandardButton.Save|StandardButton.Discard|StandardButton.Cancel
        onButtonClicked: {
            if(clickedButton === StandardButton.Save){
                if(dialogs.songTagDialog.visible){
                    dialogs.songTagDialog.set_Tag_Meta()
                }else{
                    dialogs.keyMapDialog.saveKeyMap()
                }
            }else if(clickedButton === StandardButton.Discard){
                if(dialogs.songTagDialog.visible){
                    dialogs.songTagDialog.flag = false
                    dialogs.songTagDialog.close()
                }else{
                    dialogs.keyMapDialog.getKeyMapInput()   //将快捷键还原
                    dialogs.keyMapDialog.flag = false
                    dialogs.keyMapDialog.close()
                }
            }else{
                close()
            }
        }
    }
}
