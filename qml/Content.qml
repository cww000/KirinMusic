import QtQuick 2.0
import QtQuick.Layouts 1.0
import FileIo 1.0
Item {
    width: columnLayout.width*2
    height:columnLayout.height+musicPlayer.height
    anchors.horizontalCenter: parent.horizontalCenter
    property alias musicPlayer:musicPlayer
    property alias fileNameText:fileNameText
    property alias singerText:singerText
    property alias playlistPage:playlistPage
    property alias lyricRightPage:lyricRightPage
    property alias lyricLeftPage:lyricLeftPage
    property alias leftImage:leftImage
    property alias spectrogram: spectrogram
    RowLayout{
        id:rowLayout
        Layout.alignment:Qt.AlignHCenter
        ColumnLayout{
            id:columnLayout
            Layout.alignment: Qt.AlignTop
            Image {
                id: leftImage
                Layout.topMargin: 20
                fillMode: Image.PreserveAspectCrop
                source: "qrc:/image/背景.png"
                Layout.preferredWidth: 371
                Layout.preferredHeight: 265
                cache: false
            }
            Spectrogram{
                id: spectrogram
                width: 300
                height: 130
                Layout.alignment: Qt.AlignHCenter
                anchors.topMargin: 15
            }
            LyricPage{
                id:lyricLeftPage
                visible:true
                Layout.topMargin: 15
                width: 300
                height: 90
            }

        }
        ColumnLayout{
            Layout.alignment: Qt.AlignTop
            Text {
                id: fileNameText
                Layout.preferredWidth: 300
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 20
                Layout.leftMargin: 80
                Layout.topMargin: 20
            }
            Text {
                id: singerText
                font.pointSize: 10
                visible: true
                Layout.leftMargin: 150
                Layout.topMargin: 10
            }
            LyricPage{
                id:lyricRightPage
                visible: true
                Layout.leftMargin: 70
                Layout.topMargin: 50
                width: 300
                height: 400
            }
            PlaylistPage{
                id:playlistPage
                visible: false
                Layout.leftMargin: 100
            //    Layout.topMargin: 50
            }
        }
    }
    MusicPlayer{
        id:musicPlayer
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }

    //显示本地歌词
    function showLocalLyrics(){   
        placeLyricToView()

        dialogs.lyricDialog.onClickAudioSlider()
    }

    function placeLyricToView(){
        content.lyricRightPage.lyricListView.currentIndex=-1

        if(content.playlistPage.visible) {
            content.lyricRightPage.lyricListView.visible=false
            content.lyricLeftPage.lyricListView.visible=true
        } else {
            content.lyricRightPage.lyricListView.visible=true
            content.lyricLeftPage.lyricListView.visible=false
        }

        content.lyricRightPage.lyricText.visible=false
        content.lyricRightPage.lyricListModel.clear()
        content.lyricLeftPage.lyricListModel.clear()

        dialogs.lyricDialog.lyric_id.extract_timeStamp()

        appendLyric(dialogs.lyricDialog.lyric_id.plainLyric.length)
    }

    function appendLyric(length){
        for(var i=0;i<length;i++) {
            content.lyricRightPage.lyricListModel.append({"currentLyrics":dialogs.lyricDialog.lyric_id.plainLyric[i]})
            content.lyricLeftPage.lyricListModel.append({"currentLyrics":dialogs.lyricDialog.lyric_id.plainLyric[i]})
        }
    }

    function swithToPlaylist(){
        if(!playlistPage.visible) {
            playlistPage.visible=true
            lyricLeftPage.lyricListView.visible=true
            lyricRightPage.visible=false
            lyricRightPage.lyricListView.visible=false

        }
    }


    function swithToLyric(){
        if(playlistPage.visible) {
            playlistPage.visible=false
            lyricLeftPage.lyricListView.visible=false
            lyricRightPage.visible=true
            lyricRightPage.lyricListView.visible=true

        }
    }

}
