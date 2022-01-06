import QtQuick 2.0
import KuGouSong 1.0
import KuGouMv 1.0
import KuGouPlayList 1.0
Item {
    property alias kuGouSong:kuGouSong
    property alias kuGouMv:kuGouMv
    property alias kuGouPlayList:kuGouPlayList
    KuGouSong{
        id:kuGouSong
        onSongNameChanged: {
            addSongItem()
        }
        onUrlChanged: {
            content.musicPlayer.audio.source=kuGouSong.url;

            content.musicPlayer.audio.play()
            content.musicPlayer.start.visible=false
            content.musicPlayer.pause.visible=true

            content.musicPlayer.fileName=songName[songListView.currentIndex]
            content.fileNameText.text=songName[songListView.currentIndex]
            content.singerText.text=singerName[songListView.currentIndex]

            appWindow.rootImage.source=image;
            content.leftImage.source=image;

            dialogs.miniDialog.musicStart.visible = false
            dialogs.miniDialog.musicPause.visible = true

            dialogs.lyricDialog.timerTest.running=false
            netLyric=kuGouSong.lyrics
            showNetworkLyrics();
        }
    }
    KuGouMv{
        id:kuGouMv
        onMvNameChanged: {
            addMvItem()
        }
        onMvUrlChanged: {
            videoPage.visible=true
            videoPage.video.source=mvUrl
            videoPage.video.play()
            videoPlayFlag=true
            if(content.musicPlayer.pause.visible) {
                content.musicPlayer.pause.clicked()
            }
        }
    }

    KuGouPlayList{
        id:kuGouPlayList
        onSpecialNameChanged: {
            addPlayListItem()
        }

        onSongNameChanged: {
            addSongItemInPlayList()
        }
        onUrlChanged: {
            content.musicPlayer.audio.source=kuGouPlayList.url;

            content.musicPlayer.audio.play()
            content.musicPlayer.start.visible=false
            content.musicPlayer.pause.visible=true

            content.musicPlayer.fileName=songName[songList.currentIndex]
            content.fileNameText.text=songName[songList.currentIndex]
            content.singerText.text=singerName[songList.currentIndex]

            appWindow.rootImage.source=image;
            content.leftImage.source=image;

            dialogs.miniDialog.musicStart.visible = false
            dialogs.miniDialog.musicPause.visible = true

            dialogs.lyricDialog.timerTest.running=false
            netLyric=kuGouPlayList.lyrics
            showNetworkLyrics();
        }
    }

    function addSongItemInPlayList(){
        for(var i=0;i<kuGouPlayList.songName.length;i++) {
            songModel.append({"serialNum":i+1,"songName":kuGouPlayList.singerName[i]+"-"+kuGouPlayList.songName[i],"albumName":kuGouPlayList.albumName[i]})
        }
    }

    function addSongItem(){
        bar.currentIndex=0
        var s,m;
        songListModel.clear()
        for(var i=0;i<kuGouSong.singerName.length;i++) {
            m=(kuGouSong.duration[i]-kuGouSong.duration[i]%60)/60
            s=kuGouSong.duration[i]-m*60
            if(s>=0&s<10) {
                 songListModel.append({"song":kuGouSong.singerName[i]+"-"+kuGouSong.songName[i],"album":kuGouSong.albumName[i],"duration":m+":0"+s})
            } else {
                 songListModel.append({"song":kuGouSong.singerName[i]+"-"+kuGouSong.songName[i],"album":kuGouSong.albumName[i],"duration":m+":"+s})
            }
        }
    }

    function addPlayListItem(){
        playListModel.clear()
        for(var i=0;i<kuGouPlayList.specialName.length;i++) {
            playListModel.append({"specialName":kuGouPlayList.specialName[i],"nickName":kuGouPlayList.nickName[i],"playCount":kuGouPlayList.playCount[i]})
        }
    }

    function addMvItem(){
        mvListModel.clear()
        for(var i=0;i<kuGouMv.singerName.length;i++) {
            mvListModel.append({"mvPic":kuGouMv.mvPic[i],"mvName":kuGouMv.mvName[i],"mvSinger":kuGouMv.singerName[i]})
        }
    }
}
