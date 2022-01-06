import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3
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

    Rectangle{
        id:re1
        visible: true
        ColumnLayout{
            spacing: 10
            RowLayout{
                id:rowLayout
                TextField {
                    id: inputField
                    Layout.preferredWidth: 300
                    Layout.preferredHeight: 40
                    Layout.leftMargin: (songSearchWindow.width-inputField.width)/2.5
                    focus: true
                    Layout.topMargin: 20
                    selectByMouse: true
                    font.pointSize: 12
                    placeholderText:qsTr("薛之谦")
                    background: Rectangle {
                       radius: 4
                       border.color: "black"
                    }
                    Keys.onPressed: {
                        if(event.key===Qt.Key_Return) {
                            if(inputField.text.length===0) {
                                kugou.kuGouSong.searchSong(inputField.placeholderText)
                                inputField.text=inputField.placeholderText
                            }else {
                                kugou.kuGouSong.searchSong(inputField.text)
                            }
                        }
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
                                Layout.preferredWidth: 160
                                Layout.rightMargin: 40
                                text: qsTr("歌曲名")
                                font.pixelSize: 15
                            }
                            Text {
                                Layout.preferredWidth: 120
                                Layout.rightMargin: 40
                                text: qsTr("专辑")
                                font.pixelSize: 15
                            }
                            Text {
                                Layout.preferredWidth: 80
                                text: qsTr("时长")
                                font.pixelSize: 15
                            }
                        }
                        ListView {
                            id: songListView
                            Layout.preferredWidth: songSearchWindow.width
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
                                    Layout.preferredWidth: 160
                                    Layout.rightMargin: 40
                                    elide: Text.ElideRight
                                }
                                Text {
                                    text: album
                                    Layout.preferredWidth: 120
                                    Layout.rightMargin: 40
                                    elide: Text.ElideRight
                                }
                                Text {
                                    text: duration
                                    Layout.preferredWidth: 80
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
                                    pause1,
                                    downloadSong
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
                                Layout.preferredWidth: 220
                                Layout.rightMargin: 40
                                text: qsTr("歌单名")
                                font.pixelSize: 15
                            }
                            Text {
                                Layout.preferredWidth: 120
                                Layout.rightMargin: 40
                                text: qsTr("创建人")
                                font.pixelSize: 15
                            }
                            Text {
                                Layout.preferredWidth: 60
                                text: qsTr("收藏")
                                font.pixelSize: 15
                            }
                        }
                        ListView {
                            id: playListView
                            Layout.preferredWidth: songSearchWindow.width
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
                                    Layout.preferredWidth: 220
                                    Layout.rightMargin: 40
                                    elide: Text.ElideRight
                                }
                                Text {
                                    text: nickName
                                    Layout.preferredWidth: 120
                                    Layout.rightMargin: 40
                                    elide: Text.ElideRight
                                }
                                Text {
                                    text: playCount
                                    Layout.preferredWidth: 60
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
                                onDoubleClicked: {
                                    re1.visible=false
                                    re2.visible=true
                                    songModel.clear()
                                    kugou.kuGouPlayList.getSongList(playListView.currentIndex)
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

                    GridView{
                        id: mvListView
                        anchors.fill:parent
                        anchors.leftMargin: 25
                        cellWidth: (parent.width-40)/4
                        cellHeight: parent.height/3.5
                        clip: true
                        model:mvListModel
                        delegate: mvListDelegate
                        ScrollBar.vertical: ScrollBar {
                            width: 12
                            policy: ScrollBar.AlwaysOn
                        }
                    }
                    ListModel{
                        id:mvListModel
                    }
                    Component {
                        id:mvListDelegate
                        Rectangle {
                            radius: 4
                            width: (rec3.width-40)/4
                            height: rec3.height/3.5
                            focus: true
                            color:ListView.isCurrentItem ? "lightgrey" : "white"
                            ColumnLayout{
                                id:sarchLayout
                                spacing: 5
                                Image {
                                    id: mvImage
                                    source: mvPic
                                    Layout.preferredWidth: (rec3.width-40)/4-20
                                    Layout.preferredHeight: rec3.height/3.5-50
                                    Layout.topMargin: 10
                                    fillMode: Image.PreserveAspectCrop
                                    MouseArea{
                                        id:mouseArea3
                                        anchors.fill: parent
                                        acceptedButtons: Qt.LeftButton
                                        onClicked: {
                                            if(mouse.button===Qt.LeftButton) {
                                                mvListView.currentIndex=index
                                                playVideo.trigger()
                                            }
                                        }
                                    }
                                }
                                Text {
                                    id:mvNameText
                                    text:mvName
                                    Layout.preferredWidth: (rec3.width-40)/4-20
                                    lineHeight: 0.7
                                    elide: Text.ElideRight
                                    HoverHandler{
                                        onHoveredChanged: {
                                            if(hovered) {
                                                mvNameText.color="Teal"
                                            } else {
                                                mvNameText.color="black"
                                            }
                                        }
                                    }
                                    TapHandler{
                                        onTapped: {
                                            mvListView.currentIndex=index
                                            playVideo.trigger()
                                        }
                                    }
                                }
                                Text {
                                    text: mvSinger
                                    lineHeight: 0.7
                                    color: "Teal"
                                    Layout.preferredWidth: (rec3.width-40)/4-20
                                    elide: Text.ElideRight
                                }
                            }
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

    Rectangle{
        id:re2
        visible: false
        ColumnLayout{
            spacing: 5
            RowLayout{
                spacing: 30
                Button{
                    id:backButton
                    Layout.preferredWidth: 70
                    Layout.preferredHeight: 40
                    text: "返回"
                    Layout.topMargin:10
                    onClicked: {
                        re1.visible=true
                        re2.visible=false
                    }
                }
                Text {
                    id: specialText
                    Layout.alignment: Qt.AlignCenter
                    Layout.topMargin:10
                    Layout.preferredWidth: 400
                    elide: Text.ElideRight
                    text: kugou.kuGouPlayList.specialName[playListView.currentIndex]+"--歌曲列表"
                }
            }
            Rectangle {
                id: re4
                radius: 4
                Layout.preferredWidth: songSearchWindow.width
                Layout.preferredHeight: songSearchWindow.height-backButton.height
                border.color: "black"
                ListView {
                    id: songList
                    anchors.fill: parent
                    Layout.leftMargin: 5
                    clip: true
                    spacing: 5
                    model:songModel
                    delegate: songDelegate
                    ScrollBar.vertical: ScrollBar {
                        width: 12
                        policy: ScrollBar.AlwaysOn
                    }
                }
            }
            ListModel{
                id:songModel
            }
            Component {
                id:songDelegate
                Rectangle {
                    radius: 4
                    width: songList.width - 20
                    height: 40
                    focus: true
                    color:ListView.isCurrentItem ? "lightgrey" : "white"
                    RowLayout{
                        Text {
                            text:serialNum
                            Layout.leftMargin: 10
                            Layout.preferredWidth: 50
                            Layout.rightMargin: 30
                            elide: Text.ElideRight
                        }
                        Text {
                            text: songName
                            Layout.preferredWidth: 220
                            Layout.rightMargin: 60
                            elide: Text.ElideRight
                        }
                        Text {
                            text: albumName
                            Layout.preferredWidth: 120
                            elide: Text.ElideRight
                        }
                    }
                    TapHandler{
                        id:tapHandler
                        acceptedButtons: Qt.RightButton
                        onTapped: {
                            mX=mouseArea4.mouseX
                            mY=mouseArea4.mouseY
                            menu2.open()
                        }
                    }
                    MouseArea{
                        id:mouseArea4
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton
                        onClicked: {
                            if(mouse.button===Qt.LeftButton) {
                                songList.currentIndex=index
                            }
                        }
                        onDoubleClicked: {
                            play1.trigger()
                        }
                    }
                    Menu{
                        id:menu2
                        x:mX
                        y:mY
                        contentData: [
                            play1,
                            pause1,
                            downloadSong
                        ]
                   }
                }
            }
        }
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
           if(re1.visible) {
                kugou.kuGouSong.getSongUrl(songListView.currentIndex)
           } else {
               kugou.kuGouPlayList.getSongUrl(songList.currentIndex)
           }
       }
   }
   Action{
       id:downloadSong
        text:qsTr("下载")
        onTriggered: {
            saveSongDialog.open()
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

   FileDialog{
       id: saveSongDialog
       title: "save music"
       folder:shortcuts.documents
       nameFilters: ["*.mp3"]
       selectExisting: false
       onAccepted: {
           var path=fileUrl.toString().slice(7)
           var end=path.substring(path.length-4)
           if(end!=="mp3") {
               path+=".mp3"
           }
           if(re1.visible) {
               kugou.kuGouSong.downloadSong(songListView.currentIndex,path)
           } else {
               kugou.kuGouPlayList.downloadSong(songList.currentIndex,path)
           }
       }
   }

   function showNetworkLyrics(){
       dialogs.lyricDialog.lyric_id.lyric=netLyric
       content.placeLyricToView()
       dialogs.lyricDialog.onClickAudioSlider()

   }
}

