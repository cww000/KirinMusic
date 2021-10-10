import QtQuick 2.12
import QtQuick.Layouts 1.0
import QtMultimedia 5.8
import QtQuick.Controls 2.15
Item {
    property bool wordFlag:false
    property alias wordBackground: wordBackground
    width: columnLayout.width
    height: columnLayout.height
    property alias audio:audio
    property string totalTime:"00:00";
    property string currentTime:"00:00";
    property string fileName:" "
    property alias start:start
    property alias pause:pause
    property alias cycPlay: cycPlay
    property alias loopPlay: loopPlay
    property alias ranPlay: ranPlay

    function play(index){
        dialogs.lyricDialog.testNum=0
        dialogs.songSearchDialog.networkPlay = false
        dialogs.songSearchDialog.netLyric=""
        dialogs.lyricDialog.lyric_id.lyric = ""
        dialogs.lyricDialog.timerTest.running=false
        content.singerText.text=""
        playlistPage.songListView.currentIndex=index
        dialogs.lyricDialog.fileIo.readUrls(index, dirPath+"/播放列表.txt")

        content.musicPlayer.audio.source="file://"+dialogs.lyricDialog.fileIo.source

        content.spectrogram.speTimer.running = true
        content.spectrogram.getVertices()
        dialogs.songTagDialog.showImage()

        content.lyricRightPage.lyricListModel.clear()
        content.lyricLeftPage.lyricListModel.clear()
        content.lyricRightPage.lyricListView.currentIndex=-1
        console.log(index)
        fileNameText.text=myMusicArray[index]


        dialogs.miniDialog.miniText.text=fileNameText.text

        //音乐播放及图标转换
        actions.switchToPlay()

        //查找当前播放音乐所在目录是否有对应歌词文件
        getLocalLyricFile()


        if(dialogs.lyricDialog.lyric_id.lyric === ""){
            content.lyricRightPage.lyricText.visible=true
            content.lyricRightPage.lyricListView.visible = false
            console.log("no lyric")
        }else{
            content.lyricRightPage.lyricText.visible=false
            content.lyricRightPage.lyricListView.visible = true
            console.log("has lyric")
            showLocalLyrics()
        }
    }

    //得到后缀
    function getSourceSuffix(source){
        for(var j=0;j<source.length;j++) {
            if(source[j]===".") {
                var suffix=source.slice(j)
                return suffix;
            }
        }
    }

    function getLocalLyricFile(){
        var suffix=getSourceSuffix(audio.source.toString());
        var lyricPath = audio.source.toString().slice(7).replace(suffix,'.lrc')
        dialogs.lyricDialog.fileIo.source = lyricPath
        //        console.log("lyricPath:",lyricPath)
        dialogs.lyricDialog.lyric_id.lyric=dialogs.lyricDialog.fileIo.read()
        dialogs.lyricDialog.fileIo.source = audio.source.toString().slice(7)
//                console.log("dialogs.lyricDialog.fileIo.read() in play(index) at MusicPlayer.qml",dialogs.lyricDialog.lyric_id.lyric)

    }

    function setMusicName(path){
        var newPath;
        //去掉目录
        for(var i=path.length-1;i>=0;i--) {
            if(path[i]==="/") {
                newPath=path.slice(i+1)
                break;
            }
        }
         //去掉后缀
        removeSuffix(newPath)

    }

    function removeSuffix(newPath){
        for(var j=0;j<newPath.length;j++) {
            if(newPath[j]===".") {
                fileName=newPath.slice(0,j)
                myMusicArray.push(newPath.slice(0,j))
                content.fileNameText.text=newPath.slice(0,j);
                content.singerText.text=""
                dialogs.miniDialog.miniText.text=newPath.slice(0,j);
                break;
            }
        }
    }


    //时间转化   [00:00]
    function setTime(playTime) {
        var m,s;
        var time;
        playTime=(playTime-playTime%1000)/1000;
        m=(playTime-playTime%60)/60
        s=playTime-m*60
        if(m>=0&m<10) {
            if(s>=0&s<10) {
                time="0"+m+":0"+s;
            } else {
                time="0"+m+":"+s;
            }
        } else {
            if(s>=0&s<10) {
                time=m+":0"+s;
            } else {
                time=m+":"+s;
            }
        }
        return time;
    }
    Rectangle{
        id:loopName
        width: 65
        height: 20
        color: "black"
        visible: false
        z:-1
        Text {
            id: loopNameText
            color: "white"
        }
    }
    ColumnLayout{
        id:columnLayout
        Layout.alignment: Qt.AlignBottom
        RowLayout{
            spacing: 5
            Layout.preferredHeight: 48
            Image {
                id:cycPlay         //单曲循环
                visible: false
                source: "qrc:/image/单曲循环.png"
                TapHandler{
                    onTapped: {
                        cycPlay.visible=false
                        loopPlay.visible=true
                        ranPlay.visible = false
                        loopName.visible=true
                        loopNameText.text = "列表循环"
                    }
                }
                HoverHandler{
                    onHoveredChanged: {
                        if(hovered){
                            loopName.visible = true
                            loopNameText.text = "单曲循环"
                        }
                        if(!hovered){
                            loopName.visible = false
                        }
                    }
                }
            }
            Image {
                id: loopPlay
                visible: true
                source: "qrc:/image/列表循环.png"
                TapHandler{
                    onTapped: {
                        cycPlay.visible=false
                        loopPlay.visible=false
                        ranPlay.visible = true
                        loopName.visible=true
                        loopNameText.text = "随机播放"
                    }
                }
                HoverHandler{
                    onHoveredChanged: {
                        if(hovered){
                            loopName.visible = true
                            loopNameText.text = "列表循环"
                        }
                        if(!hovered){
                            loopName.visible = false
                        }
                    }
                }
            }
            Image {
                id:ranPlay
                visible: false
                source: "qrc:/image/随机播放.png"
                TapHandler{
                    onTapped: {
                        cycPlay.visible=true
                        loopPlay.visible=false
                        ranPlay.visible = false
                        loopName.visible=true
                        loopNameText.text = "单曲循环"
                    }
                }
                HoverHandler{
                    onHoveredChanged: {
                        if(hovered){
                            loopName.visible = true
                            loopNameText.text = "随机播放"
                        }
                        if(!hovered){
                            loopName.visible = false
                        }
                    }
                }
            }
            Image {
                id: searchSongs
                source: "qrc:/image/查找.png"
                TapHandler{
                    onTapped: {
                        dialogs.songSearchDialog.visible=true
                        dialogs.songSearchDialog.network = true
                    }
                }
            }
            Rectangle{
                id:wordBackground
                border.color:"grey"
                color: "transparent"
                width:20
                height:20
                visible: true
                Text {
                    id: word1
                    anchors.centerIn: parent
                    text: qsTr("词")
                    font.pointSize: 14
                    color: "grey"
                    TapHandler{
                        onTapped: {
                            if(dialogs.miniDialog.visible){
                                wordBackground.color="transparent"
                                dialogs.miniDialog.visible = false
                            }else{
                                wordBackground.color = "lightBlue"
                                dialogs.miniDialog.visible = true
                            }
                        }
                    }
                }
            }

            Image {
                source: "qrc:/image/话筒1.png"
                visible: content.musicPlayer.fileName===" "
            }

            Image {
                source: "qrc:/image/话筒2.png"
                visible: content.musicPlayer.fileName!==" "
                TapHandler{
                    onTapped: {
                        closeAllWindow()
                        showKaraokePage()
                    }
                }
            }


            RoundButton{
                Layout.preferredWidth: 35
                Layout.preferredHeight: 35
                id:volumeButton
                icon.source:"qrc:/image/voice.png"
                onClicked: {
                    if(volumeSlider.visible) {
                        volumeSlider.visible=false;
                    } else{
                        volumeSlider.visible=true;
                    }
                }
            }
            Slider{
                id:volumeSlider
                visible: false
                to:1.0
                value: audio.volume
                Layout.preferredWidth: 100
                onValueChanged: {
                    audio.volume=value  //音量大小0-1.0
                }
            }
        }
        RowLayout{
            spacing: 15
            RoundButton{
                Layout.preferredWidth: 35
                Layout.preferredHeight: 35
                id:previous
                icon.source:"qrc:/image/last.png"
                onClicked: {
                    actions.previousAction.triggered()
                }
            }

            RoundButton{
                Layout.preferredWidth: 35
                Layout.preferredHeight: 35
                id:start
                icon.source:"qrc:/image/play1.png"
                visible: true
                onClicked: {
                    actions.playAction.triggered()
                }
            }
            RoundButton{
                Layout.preferredWidth: 35
                Layout.preferredHeight: 35
                id:pause
                icon.source:"qrc:/image/pause1.png"
                visible: false
                onClicked: {
                    actions.pauseAction.triggered()
                }
            }
            RoundButton{
                Layout.preferredWidth: 35
                Layout.preferredHeight: 35
                id:next
                icon.source:"qrc:/image/next.png"
                onClicked: {
                    actions.nextAction.triggered()
                }
            }
            Slider{
                id:audioSlider
                to:audio.duration
                value: audio.position
                Layout.preferredWidth: 500
                snapMode: Slider.SnapOnRelease
                onValueChanged: {
                    currentTime=setTime(value)
                    audio.seek(value);
                }
                onPressedChanged: {
                    if(pause.visible) {
                        if(dialogs.lyricDialog.lyric_id.lyric!=="") {
                            dialogs.lyricDialog.onClickAudioSlider()
                        }
                    }
                }
            }
            Text {
                id: playbackTime
                text:currentTime+"/"+totalTime
            }
            Image {
                id: playlist
                source: "qrc:/image/播放列表.png"
                TapHandler{
                    onTapped: {
                        if(content.playlistPage.visible) {
                            content.swithToLyric()
                        } else {
                            content.swithToPlaylist()
                        }
                    }
                }
            }
        }
    }
    Audio{
        id:audio
        onDurationChanged: {
            var playTime=audio.duration;
            totalTime=setTime(playTime);
        }
        onPositionChanged: {
            if(audio.position !== 0 && audio.position === audio.duration){
                content.lyricRightPage.lyricListModel.clear()
                content.lyricLeftPage.lyricListModel.clear()
                if(dialogs.songSearchDialog.networkPlay){
                    networkplayOrder()
                }else{
                    playOrder()
                }
            }
            content.spectrogram.pcount=position/100
        }
    }
    function playOrder() {
        var num = playlistPage.songListView.currentIndex
        if(loopPlay.visible){
            if(num === playlistPage.songListModel.count-1){
                num = 0;
                dialogs.lyricDialog.fileIo.readUrls(num, dirPath+"播放列表.txt")
                audio.source = "file://"+dialogs.lyricDialog.fileIo.source
            }else{
                num++
                dialogs.lyricDialog.fileIo.readUrls(num, dirPath+"播放列表.txt")
                audio.source = "file://"+dialogs.lyricDialog.fileIo.source
            }
        }else if(ranPlay.visible){
             var Range = playlistPage.songListModel.count-1
             var rand = Math.random();                   //random函数得到一个0-1之间的小数， round函数取整，四舍五入
             num = Math.round(rand*Range);
             dialogs.lyricDialog.fileIo.readUrls(num,dirPath+"播放列表.txt")
             audio.source = "file://"+dialogs.lyricDialog.fileIo.source
        }
        play(num)
        audio.statusChanged.connect(f)
    }

    function networkplayOrder(){
        var num = dialogs.songSearchDialog.searchlistView.currentIndex
        if(loopPlay.visible){
            if(num  === dialogs.songSearchDialog.songListModel.count-1){
                 num = 0
            }else{
                 num++;
            }
        }else if(ranPlay.visible){
            var Range = dialogs.songSearchDialog.songListModel.count-1
            var rand = Math.random();                   //random函数得到一个0-1之间的小数， round函数取整，四舍五入
            num = Math.round(rand*Range);
        }
        dialogs.songSearchDialog.searchlistView.currentIndex = num
        dialogs.songSearchDialog.play1.triggered()
    }
    function f(){
        audio.play()
    }
}
