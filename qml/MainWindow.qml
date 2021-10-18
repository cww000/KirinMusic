import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow{
    id:appWindow
    width: 870
    height: 680
    visible: true
    title: qsTr("麒麟音乐")
    property alias rootImage:name
    property var myMusicArray:[]
    property url imageUrl: ""
    property string dirPath: ""
    background: Image {
        id: name
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
        source: imageUrl
        opacity: 0.3
        cache: false
    }
    MenuSeparator{id:menuSeparator}
    menuBar: MenuBar{
        id:menuBar
        Menu{
            title: qsTr("&文件")
            contentData: [
                actions.openFileAction,
                actions.openFolderAction,
                actions.openUrlAction,
                menuSeparator,
                actions.exitAction
            ]
        }
        Menu{
            title: qsTr("&歌词")
            contentData: [
                actions.copyCurrentLyricAction,
                actions.copyAllLyricAction,
                actions.editLyricAction,
                actions.downloadLyricAction
            ]
        }
        Menu{
            title: qsTr("&播放控制")
            MenuItem{
                action: actions.playAction
            }
            MenuItem{
                action: actions.pauseAction
            }
            MenuItem{
                action: actions.previousAction
            }
            MenuItem{
                action: actions.nextAction
            }
            MenuItem{
                action: actions.fastforwardfiveScdAction
            }
            MenuItem{
                action: actions.backfiveScdAction
            }
            MenuSeparator { }

            Menu {
                title: "循环模式"
                MenuItem{
                    action: actions.cycPlayAction
                }
                MenuItem{
                    action: actions.loopPlayAction
                }
                MenuItem{
                    action: actions.ranPlayAction
                }
            }
        }

        Menu{
            title: qsTr("&播放列表")
            contentData: [
                actions.deleteAction,
                actions.songSearchAction
            ]
        }

        Menu{
            title: qsTr("&工具")
            contentData: [
                actions.recentlyPlayAction,
                actions.trackInformationAction,
                actions.skinAction,
                actions.keyMapAction
            ]
        }
        Menu{
            title: qsTr("&帮助")
            contentData: [
                actions.aboutAction
            ]
        }
    }
    Content{
        id:content
    }
    KaraokePage{
        id:karaokePage
        visible: false
    }

    Actions{
        id:actions
    }
    Dialogs{
        id:dialogs
        fileMusicDialog.onAccepted: {
            onAcceptedfileMusicDialog();
        }
        folderMusicDialog.onAccepted: {
            onAcceptedFolderMusicDialog()
        }
    }


    function closeAllWindow(){
        dialogs.lyricDialog.visible=false
        dialogs.lyricSearchDialog.visible=false
        dialogs.songSearchDialog.visible=false
        dialogs.recentlyPlayDialog.visible=false
        dialogs.songTagDialog.visible=false
        dialogs.keyMapDialog.visible=false
        dialogs.miniDialog.visible=false
    }

    function showKaraokePage(){
        actions.pauseAction.triggered()
        content.visible=false
        karaokePage.visible=true
        menuBar.visible=false
        appWindow.title="K歌"

        karaokePage.karaoke.search(content.fileNameText.text)
        console.log()
    }

    Component.onCompleted: {
        dirPath = dialogs.lyricDialog.fileIo.dirPath
        var i = 0;
        dialogs.lyricDialog.fileIo.getPlaylist();
        var length=dialogs.lyricDialog.fileIo.urls.length

        for(i=0;i<length;i++) {
            content.musicPlayer.setMusicName(dialogs.lyricDialog.fileIo.urls[i]);  //得到歌曲名字
            content.playlistPage.songListModel.append({"chapter":content.musicPlayer.fileName})
        }
        content.musicPlayer.audio.source="file://"+dialogs.lyricDialog.fileIo.urls[length-1]

        content.musicPlayer.getLocalLyricFile()
        if(dialogs.lyricDialog.lyric_id.lyric!=="") {
            content.placeLyricToView()    //将歌词放到视图中
        } else {
            content.lyricRightPage.lyricText.visible=true
        }

        content.playlistPage.songListView.currentIndex =length-1
        if(length!==0) {
            content.spectrogram.getVertices();
        } else {
            content.lyricRightPage.lyricText.visible=false
        }

        //背景设置
        dialogs.lyricDialog.fileIo.readBackgroundUrl(dirPath+"/背景图.txt")
        dialogs.skinDialog.usingImage = dialogs.lyricDialog.fileIo.source
        imageUrl = dialogs.skinDialog.usingImage
        dialogs.songTagDialog.showImage()

        actions.getKeyMap()   //获取快捷键设置
    }

    function onAcceptedFolderMusicDialog(){
        actions.pauseAction.triggered()
        var folderPath=dialogs.folderMusicDialog.folder.toString().slice(7);
        var filePaths=dialogs.lyricDialog.fileIo.getFiles(folderPath);
        if(filePaths.length!==0) {
            for(var i=0;i<filePaths.length;i++) {
                content.musicPlayer.removeSuffix(filePaths[i])
                 content.playlistPage.songListModel.append({"chapter":content.musicPlayer.fileName})
            }

            var path=new Array;
            for(var j=0;j<filePaths.length;j++) {
                path[j]=dialogs.lyricDialog.fileIo.strToUrl(dialogs.folderMusicDialog.folder.toString()+"/"+filePaths[j]);
            }

            dialogs.lyricDialog.fileIo.saveUrls(path)  //加入到播放列表
            content.musicPlayer.audio.source=path[path.length-1];
            dialogs.songSearchDialog.close()
            showInformation()

        }
    }

    function onAcceptedfileMusicDialog(){
        actions.pauseAction.triggered()
        var path;
        if(dialogs.fileMusicDialog.fileUrls.length<=1) {
            path=dialogs.fileMusicDialog.fileUrl.toString().slice(7);
            //去掉播放列表中已经存在的相同地址的歌曲
            if(dialogs.lyricDialog.fileIo.isExist(path)) {
                content.musicPlayer.audio.source=dialogs.fileMusicDialog.fileUrl
                content.musicPlayer.setMusicName(path);  //得到歌曲名字
                content.playlistPage.songListModel.append({"chapter":content.musicPlayer.fileName})
                dialogs.lyricDialog.fileIo.saveUrls(dialogs.fileMusicDialog.fileUrls)  //加入到播放列表
                dialogs.songSearchDialog.close()
                showInformation()
            }
        } else {
            var lastAdd=-1;
            var addUrl=new Array;
            for(var i=0;i<dialogs.fileMusicDialog.fileUrls.length;i++) {
                path=dialogs.fileMusicDialog.fileUrls[i].toString().slice(7);
                if(dialogs.lyricDialog.fileIo.isExist(path)) {
                    lastAdd=i;
                    addUrl.push(dialogs.fileMusicDialog.fileUrls[i])
                    content.musicPlayer.setMusicName(dialogs.fileMusicDialog.fileUrls[i].toString().slice(7));
                    content.playlistPage.songListModel.append({"chapter":content.musicPlayer.fileName})
                }
            }
            if(lastAdd!==-1) {
                dialogs.lyricDialog.fileIo.saveUrls(addUrl)
                content.musicPlayer.audio.source=addUrl[addUrl.length-1]
                dialogs.songSearchDialog.close()
                showInformation()
            }
        }
    }

    function showInformation() {
        content.playlistPage.songListView.currentIndex = content.playlistPage.songListModel.count-1
        content.lyricRightPage.lyricText.visible=true

        content.spectrogram.speTimer.running = false
        content.spectrogram.getVertices()
        dialogs.songTagDialog.showImage()

        //判断打开音乐之前是否存在歌词，以及打开的音乐是否有歌词
        del_and_add_lyric()
        dialogs.songSearchDialog.networkPlay=false
    }

    function del_and_add_lyric(){
        if(dialogs.lyricDialog.lyric_id.lyric!=="") {
            dialogs.lyricDialog.timerTest.running=false
            content.lyricRightPage.lyricListModel.clear()
            content.lyricLeftPage.lyricListModel.clear()
        }

        content.musicPlayer.getLocalLyricFile()
        if(dialogs.lyricDialog.lyric_id.lyric!=="") {
            content.placeLyricToView()    //将歌词放到视图中
        } else {
            content.lyricRightPage.lyricText.visible=true
        }
    }
}
