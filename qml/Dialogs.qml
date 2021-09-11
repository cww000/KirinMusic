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
    property alias songTagDialog:songTagDialog
    property alias lyricDialog:lyricDialog
    property alias recentlyPlayDialog: recentlyPlayDialog
    property alias keyMapDialog:keyMapDialog
    property alias lyricSearchDialog:lyricSearchDialog
    property alias songSearchDialog:songSearchDialog
    property alias miniDialog: miniDialog
    property alias saveDialog: saveDialog
  //  property alias karaokeDialog:karaokeDialog


    QQ.FileDialog{
        id:fileMusicDialog
        title: "Please choose a music file"
        folder:shortcuts.documents
        nameFilters: ["Audio Files(*.mp3 *.ogg *.flac)"]
        selectMultiple: true
    }

    FolderDialog{
        id:folderMusicDialog
        title:"select a folder"
    }
    QQ.MessageDialog{
        id:aboutDialog
        title:"May I have your attention please"
        text:"The name of the program is DaliyMusic
              The authors of the program are wenwenChen、haiyanNie、chunlinLi
              This program can realize the music playing at the same time the lyrics for editing,editing the song meta information"
        standardButtons: StandardButton.Ok
    }

//    Karaoke{
//        id:karaokeDialog
//        visible: false
//    }

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
    QQ.MessageDialog{
        id: saveDialog
        title: qsTr("hint")
        text: qsTr("The information has been modified, do you want to save?")
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
                    dialogs.keyMapDialog.flag = false
                    dialogs.keyMapDialog.close()
                }
            }else{
                close()
            }
        }
    }
}
