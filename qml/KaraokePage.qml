import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtMultimedia 5.8
import  Karaoke 1.0
import QtQml.Models 2.15
import QtGraphicalEffects 1.0
import Lyric 1.0
import AudioRecorder 1.0
import QtQuick.Dialogs 1.0

Item{
    id:karaokeWindow
    width:870
    height:680
    visible: true
    property string  currentTime:content.musicPlayer.setTime(karaokeAudio.position)
    property string  totalTime:content.musicPlayer.setTime(karaokeAudio.duration)
    property alias songName:songName
    property alias karaoke:karaoke
    property int timeStampIndex:0
    property bool timeFlag:false


    ColumnLayout{
        spacing: 10
        Text {
            id: songName
            font.pointSize: 20
            text: content.fileNameText.text
            Layout.leftMargin: (appWindow.width-songName.width)/2
        }
        Text {
            id: time
            text:"正在录制 "+currentTime+"/"+totalTime
            font.pointSize: 10
            Layout.leftMargin: (appWindow.width-time.width)/2
        }

        ListView{
            id:karaokeLyricView
            visible:true
            Layout.topMargin: 30
            width: 300
            height: 400
            spacing: 30
            currentIndex: -1
            //固定currentItem的位置
            highlightRangeMode:ListView.ApplyRange
            preferredHighlightBegin:height/2
            preferredHighlightEnd: height/2+30
            model:ListModel{
                id:karaokeLyricModel
            }
            delegate: Text {
                id: mt
                text: currentLyrics
                width: 300
                font.pixelSize: ListView.isCurrentItem ? 25 : 18
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                 opacity: 0.6
                color: ListView.isCurrentItem ? "red" : "black"
                visible: true
                onColorChanged: {
                    ani.running=true
                }
                PropertyAnimation{
                    id:ani
                    target: mr1
                    property: "width"
                    from: 0
                    to: m2.width
                    duration: 4000
                    loops: 1
                    running: false
                }
                Rectangle{
                    id: m2
                    anchors.fill: mt
                    color:"#00000000"
                    Rectangle{
                        id: mr1
                        height:parent.height
                        color: "red"
                        //width是从0到m2.width
                    }
                    Rectangle{
                        x: mr1.width //从0到m2.width
                        height:parent.height
                        width: m2.width-mr1.width
                        color: "lightgrey"
                    }
                }
                ShaderEffectSource {       //ShaderEffectSource类型将sourceItem渲染成纹理
                    id: mask
                    visible: false
                    anchors.fill: mt
                    hideSource : true
                    sourceItem: m2
                }
                Blend {
                    anchors.fill: mt
                    source: mt
                    foregroundSource: mask
                    mode: "color"
                }
            }
            Layout.leftMargin: (appWindow.width-karaokeLyricView.width)/2
        }

        RowLayout{
            id:controlButton
            Layout.leftMargin: (appWindow.width-controlButton.width)/2
            Layout.topMargin: 10
            spacing: 20
            Image{
                id:goBack
                source: "qrc:/image/返回.png"
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30
                TapHandler{
                    onTapped: {
                        moveToContent()
                    }
                }
            }

            Image{
                id:bz
                source: "qrc:/image/伴奏.png"
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30
                TapHandler{
                    onTapped: {
                        switchToYc()
                    }
                }
            }

            Image{
                id:yc
                source: "qrc:/image/原唱.png"
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30
                visible: false
                TapHandler{
                    onTapped: {
                        switchToBz()
                    }
                }
            }


            Image{
                id:pauseButton
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30
                source: "qrc:/image/开始录制.png"
                visible: false
                TapHandler{
                    onTapped: {
                        switchToPause()
                        audioRecorder.stopRecord()
                    }
                }
            }

            Image{
                id:recordButton
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30
                source: "qrc:/image/红点.png"
                visible: true
                TapHandler{
                    onTapped: {
                        switchToPlay()
                        audioRecorder.startRecord()
                    }
                }
            }

            Image{
                id:singAgainButton
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30
                source: "qrc:/image/红点.png"
                visible: true
                TapHandler{
                    onTapped: {
                        karaokeAudio.seek(0)
                        switchToPause()
                        audioRecorder.stopRecord()
                        switchToPlay()
                        audioRecorder.startRecord()
                    }
                }
            }

            Image{
                id:saveButton
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30
                source: "qrc:/image/保存.png"
                TapHandler{
                    onTapped: {
                        switchToPause()
                        audioRecorder.stopRecord()
                        fileDialog.open()
                    }
                }
            }
        }
    }

    Audio{
        id:karaokeAudio
    }

    FileDialog{
        id: fileDialog
        title: "Please choose a music file"
        folder:shortcuts.documents
        nameFilters: ["*.wav"]
        selectExisting: false
        onAccepted: {
            audioRecorder.save(fileUrl.toString().slice(7))
        }
    }

    Karaoke{
        id:karaoke
        onUrlChanged:{
            console.log(url)
            karaokeAudio.source=url
            showKaraokeLyrics()
        }
    }

    AudioRecorder{
        id:audioRecorder
    }

    Lyric{
        id:karaokeLyric
    }

    Timer{
        id:karaokeTimer
        running: false
        repeat: true
        onTriggered: {
            if(timeFlag) {
                //定时器第一次被触发
                timeFlag=false
                karaokeLyricView.currentIndex=timeStampIndex
            } else {
                karaokeLyricView.currentIndex++;
            }
            timeStampIndex++;

            karaokeTimer.interval=karaokeLyric.timeStamp[timeStampIndex]-karaokeLyric.timeStamp[timeStampIndex-1];
            if(currentTime===totalTime) {
                karaokeTimer.running=false
                switchToPause()
            }

        }
    }

    function switchToBz(){
        bz.visible=true
        yc.visible=false
        switchToPause()       //转换图标，并停止当前录制
        audioRecorder.stopRecord()
        karaokeAudio.source=karaoke.url
        karaokeAudio.seek(0)     //source置0,重新开始录制
    }

    function switchToYc(){
        yc.visible=true
        bz.visible=false
        switchToPause()      //转换图标，并停止当前录制
        audioRecorder.stopRecord()
        karaokeAudio.source=content.musicPlayer.audio.source
        karaokeAudio.seek(0)
    }


    function switchToPlay(){
        recordButton.visible=false
        pauseButton.visible=true
        karaokeAudio.play()
        timerStart()
    }

    function switchToPause(){
        recordButton.visible=true
        pauseButton.visible=false
        karaokeAudio.pause()
        karaokeTimer.running=false
    }

    function showKaraokeLyrics(){
        karaokeLyric.lyric=karaoke.lyrics
        karaokeLyricModel.clear()
        karaokeLyric.extract_timeStamp()

        var length=karaokeLyric.plainLyric.length
        for(var i=0;i<length;i++) {
            karaokeLyricModel.append({"currentLyrics":karaokeLyric.plainLyric[i]})
        }
    }

    function timerStart(){
        if("["+currentTime+"]" !=="[00:00]") {
            timeStampIndex=karaokeLyric.findTimeInterval("["+currentTime+"]");
        } else {
            timeStampIndex=karaokeLyric.findTimeInterval("00:00.01");
        }

        karaokeTimer.interval=karaokeLyric.timeDif;     //设置第一个时间间隔
        timeFlag=true;
        karaokeTimer.running=true
    }

    function moveToContent(){
        if(pauseButton.visible) {
            switchToPause()
        }
        karaokePage.visible=false
        content.visible=true
        menuBar.visible=true
        karaokeLyricModel.clear()
        karaokeAudio.source=""
        karaokeLyric.lyric=""    //返回的时候要将歌词清空
        appWindow.title="KirinMusic" 
    }
}
