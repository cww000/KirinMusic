import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtMultimedia 5.15
ApplicationWindow{
    width:600
    height:500
    title: qsTr("搜索歌曲")
    visible: true
    id:songSearchWindow
    property bool networkPlay:false
    property bool network:false
    property bool videoPlayFlag: false
    property double mX:0.0
    property double mY:0.0
    property alias songListView: songListView
    property alias play1: play1
    property alias pauseVideo: pauseVideo
    property alias songListModel: songListModel
    property alias inputField: inputField
    property string netLyric:""

    background: Image {
        id: name
        fillMode: Image.PreserveAspectCrop
        source: imageUrl
        anchors.fill: parent
        opacity: 0.3
    }

    ColumnLayout{
        spacing: 10
        RowLayout{
            id:rowLayout
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
            }
            Button {
                id:songSearchButton
                text: qsTr("搜索")
                Layout.preferredWidth: 70
                Layout.preferredHeight: 40
                Layout.topMargin: 20
                onClicked: {
                    if(inputField.text.length===0) {
                        kugou.kuGouSong.searchSong(inputField.placeholderText)
                        inputField.text=inputField.placeholderText
                    } else {
                        kugou.kuGouSong.searchSong(inputField.text)
                    }
                }
            }
        }

        TabBar {
            id: bar
            Layout.preferredWidth: parent.width/3
            TabButton {
                text: qsTr("单曲")
                implicitHeight: 30
                implicitWidth: 10
                onClicked: {
                    if(inputField.text.length===0) {
                        kugou.kuGouSong.searchSong(inputField.placeholderText)
                        inputField.text=inputField.placeholderText
                    } else {
                        kugou.kuGouSong.searchSong(inputField.text)
                    }
                }
            }
            TabButton {
                text: qsTr("歌单")
                implicitHeight:30
                implicitWidth: 10
                onClicked: {
                    if(inputField.text.length===0) {
                        kugou.kuGouPlayList.searchPlayList(inputField.placeholderText)
                        inputField.text=inputField.placeholderText
                    } else {
                        kugou.kuGouPlayList.searchPlayList(inputField.text)
                    }
                }
            }
            TabButton{
                text: qsTr("mv")
                implicitHeight: 30
                implicitWidth: 10
                onClicked: {
                    if(inputField.text.length===0) {
                        kugou.kuGouMv.searchMv(inputField.placeholderText)
                        inputField.text=inputField.placeholderText
                    } else {
                        kugou.kuGouMv.searchMv(inputField.text)
                    }
                }
            }
        }

        StackLayout {
            id: stack
            width: parent.width
            height: parent.height-bar.height-rowLayout-15
            currentIndex: bar.currentIndex
            Rectangle {
                id: rec1
                radius: 4
                Layout.preferredWidth: songSearchWindow.width
                Layout.preferredHeight: songSearchWindow.height-inputField.height
                visible:false
                border.color: "black"

                ColumnLayout{
                    anchors.fill: parent
                    RowLayout{
                        id: row1
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
                            text: qsTr("时长")
                            font.pixelSize: 15
                        }
                    }
                    ListView {
                        id: songListView
                        Layout.preferredWidth: parent.width
                        Layout.leftMargin: 5
                        Layout.preferredHeight: parent.height-row1.height
                        clip: true
                        spacing: 5
                        model:songListModel
                        delegate: songListDelegate
                        ScrollBar.vertical: ScrollBar {
                            width: 12
                            policy: ScrollBar.AlwaysOn
                        }
                    }
                }
                ListModel{
                    id:songListModel
                }
                Component {
                    id:songListDelegate
                    Rectangle {
                        radius: 4
                        width: songListView.width - 20
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
                                mX=mouseArea1.mouseX
                                mY=mouseArea1.mouseY
                                menu1.open()
                            }
                        }
                        MouseArea{
                            id:mouseArea1
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton
                            onClicked: {
                                if(mouse.button===Qt.LeftButton) {
                                    songListView.currentIndex=index
                                }
                            }
                            onDoubleClicked: {
                                play1.trigger()
                            }
                        }
                        Menu{
                            id:menu1
                            x:mX
                            y:mY
                            contentData: [
                                play1,
                                pause1
                            ]
                       }
                    }
                }
            }

            Rectangle {
                id: rec2
                radius: 4
                Layout.preferredWidth: songSearchWindow.width
                Layout.preferredHeight: songSearchWindow.height-inputField.height
                visible:false
                border.color: "black"

                ColumnLayout{
                    anchors.fill: parent
                    RowLayout{
                        id: row2
                        Layout.fillWidth: true
                        Layout.leftMargin: 5
                        Text {
                            Layout.preferredWidth: 120
                            Layout.rightMargin: 50
                            text: qsTr("歌单")
                            font.pixelSize: 15
                        }
                        Text {
                            Layout.preferredWidth: 120
                            Layout.rightMargin: 60
                            text: qsTr("创建人")
                            font.pixelSize: 15
                        }
                        Text {
                            Layout.preferredWidth: 120
                            text: qsTr("收藏")
                            font.pixelSize: 15
                        }
                    }
                    ListView {
                        id: playListView
                        Layout.preferredWidth: parent.width
                        Layout.leftMargin: 5
                        Layout.preferredHeight: parent.height-row2.height
                        clip: true
                        spacing: 5
                        model:playListModel
                        delegate: playListDelegate
                        ScrollBar.vertical: ScrollBar {
                            width: 12
                            policy: ScrollBar.AlwaysOn
                        }
                    }
                }
                ListModel{
                    id:playListModel
                }
                Component {
                    id:playListDelegate
                    Rectangle {
                        radius: 4
                        width: playListView.width - 20
                        height: 40
                        focus: true
                        color:ListView.isCurrentItem ? "lightgrey" : "white"
                        RowLayout{
                            id:sarchLayout
                            Text {
                                text:specialName
                                Layout.preferredWidth: 120
                                Layout.rightMargin: 50
                                elide: Text.ElideRight
                            }
                            Text {
                                text: nickName
                                Layout.preferredWidth: 120
                                Layout.rightMargin: 60
                                elide: Text.ElideRight
                            }
                            Text {
                                text: playCount
                                Layout.preferredWidth: 120
                                Layout.rightMargin: 20
                            }

                        }
                        MouseArea{
                            id:mouseArea2
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton
                            onClicked: {
                                if(mouse.button===Qt.LeftButton) {
                                    playListView.currentIndex=index
                                }
                            }
                        }
                    }
                }
            }

            Rectangle {
                id: rec3
                radius: 4
                Layout.preferredWidth: songSearchWindow.width
                Layout.preferredHeight: songSearchWindow.height-inputField.height
                visible:false
                border.color: "black"

                ColumnLayout{
                    anchors.fill: parent
                    RowLayout{
                        id: row3
                        Layout.fillWidth: true
                        Layout.leftMargin: 5
                        Text {
                            Layout.preferredWidth: 120
                            Layout.rightMargin: 50
                            text: qsTr("mv")
                            font.pixelSize: 15
                        }
                        Text {
                            Layout.preferredWidth: 120
                            Layout.rightMargin: 60
                            text: qsTr("歌手")
                            font.pixelSize: 15
                        }
                        Text {
                            Layout.preferredWidth: 120
                            text: qsTr("时长")
                            font.pixelSize: 15
                        }
                    }
                    ListView {
                        id: mvListView
                        Layout.preferredWidth: parent.width
                        Layout.leftMargin: 5
                        Layout.preferredHeight: parent.height-row3.height
                        clip: true
                        spacing: 5
                        model:mvListModel
                        delegate: mvListDelegate
                        ScrollBar.vertical: ScrollBar {
                            width: 12
                            policy: ScrollBar.AlwaysOn
                        }
                    }
                }
                ListModel{
                    id:mvListModel
                }
                Component {
                    id:mvListDelegate
                    Rectangle {
                        radius: 4
                        width: playListView.width - 20
                        height: 40
                        focus: true
                        color:ListView.isCurrentItem ? "lightgrey" : "white"
                        RowLayout{
                            id:sarchLayout
                            Text {
                                text:mvName
                                Layout.preferredWidth: 120
                                Layout.rightMargin: 50
                                elide: Text.ElideRight
                            }
                            Text {
                                text: mvSinger
                                Layout.preferredWidth: 120
                                Layout.rightMargin: 60
                                elide: Text.ElideRight
                            }
                            Text {
                                text: duration
                                Layout.preferredWidth: 120
                                Layout.rightMargin: 20
                            }

                        }
                        TapHandler{
                            id:tapHandler
                            acceptedButtons: Qt.RightButton
                            onTapped: {
                                mX=mouseArea3.mouseX
                                mY=mouseArea3.mouseY
                                menu3.open()
                            }
                        }
                        MouseArea{
                            id:mouseArea3
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton
                            onClicked: {
                                if(mouse.button===Qt.LeftButton) {
                                    mvListView.currentIndex=index
                                }
                            }
                            onDoubleClicked: {
                                mvListView.currentIndex=index
                                playVideo.trigger()
                            }
                        }
                        Menu{
                            id:menu3
                            x:mX
                            y:mY
                            contentData: [
                                playVideo,
                                pauseVideo
                            ]
                       }
                    }
                }
            }
        }
    }
    onClosing: {
        songListModel.clear()
        playListModel.clear()
        mvListModel.clear()
        network = false
        inputField.clear();
    }
    KuGou{
        id:kugou
    }
    VideoPage{
        id:videoPage
    }

   Action{
       id:play1
       text: qsTr("播放")
       onTriggered: {
           if(videoPlayFlag) {
               pauseVideo.trigger()
               console.log(videoPlayFlag)
           }
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
           kugou.kuGouSong.getSongUrl(songListView.currentIndex)
       }
   }
   Action{
       id:playVideo
       text: qsTr("播放")
       onTriggered: {
           kugou.kuGouMv.getMvUrl(mvListView.currentIndex)
       }
   }

   Action{
       id:pauseVideo
       text: qsTr("暂停")
       onTriggered: {
           videoPlayFlag=false
           videoPage.video.pause()
       }
   }

   Action{
       id:pause1
       text: qsTr("暂停")
       onTriggered: actions.pauseAction.triggered()
   }

   function showNetworkLyrics(){
       dialogs.lyricDialog.lyric_id.lyric=netLyric
       content.placeLyricToView()
       dialogs.lyricDialog.onClickAudioSlider()

   }
}

