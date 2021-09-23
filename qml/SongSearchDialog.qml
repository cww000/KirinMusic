import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import KuGou 1.0
ApplicationWindow{
    width:600
    height:500
    title: qsTr("搜索歌曲")
    visible: true
    id:songSearchWindow
    property bool networkPlay:false
    property double mX:0.0
    property double mY:0.0
    property alias searchlistView: listView
    property alias play1: play1
    property alias songListModel: songListmodel
    property alias inputField: inputField

    background: Image {
        id: name
        fillMode: Image.PreserveAspectCrop
        source: "qrc:/image/背景.png"
        anchors.fill: parent
        opacity: 0.3
    }

    ColumnLayout{
        spacing: 10
        RowLayout{
            TextField {
                id: inputField
                Layout.preferredWidth: 300
                Layout.preferredHeight: 40
                Layout.leftMargin: (songSearchWindow.width-inputField.width)/3
                focus: true
                Layout.topMargin: 20
                selectByMouse: true
                font.pointSize: 12
                placeholderText:qsTr("王贰浪")
                background: Rectangle {
                   radius: 4
                   border.color: "black"
                }
                Keys.onPressed: {
                    if(event.key===Qt.Key_Return) {
                        sarchList.visible=true
                        if(inputField.text.length===0) {
                            kugou.search(inputField.placeholderText)
                        } else {
                            kugou.search(inputField.text)
                        }
                    }
                }

            }
            Button {
                id:songSarchBtn
                text: qsTr("搜索")
                Layout.preferredWidth: 70
                Layout.preferredHeight: 40
                Layout.topMargin: 20
                onClicked: {
                    sarchList.visible=true
                    if(inputField.text.length===0) {
                        kugou.search(inputField.placeholderText)
                    } else {
                        kugou.search(inputField.text)
                    }
                }
            }
        }

        Rectangle {
            id: sarchList
            radius: 4
            Layout.preferredWidth: songSearchWindow.width
            Layout.preferredHeight: songSearchWindow.height-inputField.height
            visible:false
            border.color: "black"

            ColumnLayout{
                anchors.fill: parent
                RowLayout{
                    id: row
                    Layout.fillWidth: true
                    Layout.leftMargin: 5
                    Text {
                        Layout.preferredWidth: 120
                        Layout.rightMargin: 50
                        text: qsTr("歌曲")
                        font.pixelSize: 15
                    }
                    Text {
                        Layout.preferredWidth: 120
                        Layout.rightMargin: 60
                        text: qsTr("专辑")
                        font.pixelSize: 15
                    }
                    Text {
                        Layout.preferredWidth: 120
                        text: qsTr("时间")
                        font.pixelSize: 15
                    }
                }
                ListView {
                    id: listView
                    Layout.preferredWidth: parent.width
                    Layout.leftMargin: 5
                    Layout.preferredHeight: parent.height-row.height
                    clip: true
                    spacing: 5
                    model:songListmodel
                    delegate: songListDelegate
                    ScrollBar.vertical: ScrollBar {
                        width: 12
                        policy: ScrollBar.AlwaysOn
                    }
                }
            }
            ListModel{
                id:songListmodel
            }
            Component {
                id:songListDelegate
                Rectangle {
                    radius: 4
                    width: listView.width - 20
                    height: 40
                    focus: true
                    color:ListView.isCurrentItem ? "lightgrey" : "white"
                    RowLayout{
                        id:sarchLayout
                        Text {
                            text:song
                            Layout.preferredWidth: 120
                            Layout.rightMargin: 50
                            elide: Text.ElideRight
                        }
                        Text {
                            text: album
                            Layout.preferredWidth: 120
                            Layout.rightMargin: 60
                            elide: Text.ElideRight
                        }
                        Text {
                            text: duration
                            Layout.preferredWidth: 120
                            Layout.rightMargin: 20
                        }
                        Text {
                            id: kugouText
                            text: qsTr("酷狗音乐")
                            Layout.preferredWidth:60
                        }
                    }
                    TapHandler{
                        id:tapHandler
                        acceptedButtons: Qt.RightButton
                        onTapped: {
                            mX=mouseArea.mouseX
                            mY=mouseArea.mouseY
                            menu1.open()
                        }
                    }
                    MouseArea{
                        id:mouseArea
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton
                        onClicked: {
                            if(mouse.button===Qt.LeftButton) {
                                listView.currentIndex=index
                            }
                        }
                        onDoubleClicked: {
                            if(mouse.button==Qt.LeftButton) {
                                play1.triggered();
                            }
                        }

                    }
                    Menu{
                        id:menu1
                        x:mX
                        y:mY
                        contentData: [
                            play1,
                            pause1,
                        ]
                   }
                }
            }
        }
    }
    onClosing: {
        songListModel.clear()
        sarchList.visible=false
        networkPlay = false
        inputField.clear();
    }
    KuGou{
        id:kugou
        onSingerNameChanged: {
            var s,m;
            songListmodel.clear()
            for(var i=0;i<singerName.length;i++) {
                m=(duration[i]-duration[i]%60)/60
                s=duration[i]-m*60
                if(s>=0&s<10) {
                     songListmodel.append({"song":singerName[i]+"-"+songName[i],"album":albumName[i],"duration":m+":0"+s})
                } else {
                     songListmodel.append({"song":singerName[i]+"-"+songName[i],"album":albumName[i],"duration":m+":"+s})
                }
            }
        }
        onUrlChanged: {
            content.musicPlayer.audio.source=url;

            content.musicPlayer.audio.play()
            content.musicPlayer.start.visible=false
            content.musicPlayer.pause.visible=true

            content.musicPlayer.fileName=songName[listView.currentIndex]
            content.fileNameText.text=songName[listView.currentIndex]
            content.singerText.text=singerName[listView.currentIndex]

            appWindow.rootImage.source=image;
            content.leftImage.source=image;

            dialogs.miniDialog.musicStart.visible = false
            dialogs.miniDialog.musicPause.visible = true

            dialogs.lyricDialog.timerTest.running=false
            showNetworkLyrics();
        }
    }

   Action{
       id:play1
       text: qsTr("播放")
       onTriggered: {
           networkPlay=true
           content.spectrogram.speTimer.running = false
           content.spectrogram.canvasClear()
           content.lyricRightPage.lyricListModel.clear()
           content.lyricLeftPage.lyricListModel.clear()

           if(dialogs.lyricDialog.timerTest.running) {
               dialogs.lyricDialog.testNum=0     //让testArea中的歌词不再高亮
               dialogs.lyricDialog.timerTest.running=false;
               dialogs.lyricDialog.action.addTagAction.enabled=true;
               dialogs.lyricDialog.action.deleteHeaderLabelAction.enabled=true;
               dialogs.lyricDialog.action.deleteAllLabelAction.enabled=true;
               dialogs.lyricDialog.toolBarAddTag.enabled=true
               dialogs.lyricDialog.tooBarDeleteHeaderLabel.enabled=true
           }
           kugou.onclickPlay(listView.currentIndex)
       }
   }
   Action{
       id:pause1
       text: qsTr("暂停")
       onTriggered: actions.pauseAction.triggered()
   }

   function showNetworkLyrics(){
       dialogs.lyricDialog.lyric_id.lyric=kugou.lyrics
       content.placeLyricToView()
       dialogs.lyricDialog.onClickAudioSlider()

   }

}

