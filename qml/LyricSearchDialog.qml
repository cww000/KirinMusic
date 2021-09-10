import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import LyricDownload 1.0
import QtQuick.Dialogs 1.3
ApplicationWindow{
    width:500
    height:400
    id:lyricSearchWindow
    title:qsTr("下载歌词")
    background: Image {
        source: "qrc:/image/背景.png"
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
        opacity: 0.3
    }
    Action{
        id:download
        text: qsTr("下载")
        icon.source: "qrc:/image/download.png"
        onTriggered: {
            lyricDownload.onClickDownload(listView.currentIndex)
        }
    }
    property real mX: 0.0
    property real mY: 0.0

    property alias lyricDownload: lyricDownload
    ColumnLayout{
        spacing: 10
        RowLayout{
            spacing: 10
            TextField {
                id: keywordInput
                focus: true
                Layout.preferredWidth: 300
                Layout.preferredHeight: 40
                Layout.topMargin: 20
                Layout.leftMargin: (lyricSearchWindow.width-keywordInput.width)/3
                selectByMouse: true
                font.pointSize: 12
                placeholderText:qsTr("say hello")
                background: Rectangle {
                   radius: 4
                   border.color: "black"
                }
                Keys.onPressed: {
                    if(event.key===Qt.Key_Return) {
                        if(keywordInput.text.length===0) {
                            lyricDownload.lyricSearch(placeholderText)
                        }else {
                            lyricDownload.lyricSearch(text)
                        }
                        searchList.visible=true
                    }
                }
            }

            Button {
                id:searchBtn
                text: qsTr("搜索")
                Layout.preferredWidth: 70
                Layout.preferredHeight: 40
                Layout.topMargin: 20
                onClicked: {
                    if(keywordInput.text.length===0) {
                        lyricDownload.lyricSearch(keywordInput.placeholderText)
                    }else {
                        lyricDownload.lyricSearch(keywordInput.text)
                    }
                    searchList.visible=true
                }
            }
        }
        Rectangle {
            id: searchList
            radius: 4
            Layout.preferredWidth: lyricSearchWindow.width
            Layout.preferredHeight: lyricSearchWindow.height-searchBtn.height
            visible:false
            border.color: "black"
            ColumnLayout{
                anchors.fill: parent
                RowLayout{
                    id: row
                    Layout.fillWidth: true
                    Layout.leftMargin: 5
                    Text {
                        Layout.preferredWidth: 100
                        Layout.rightMargin: 40
                        text: qsTr("歌曲")
                        font.pixelSize: 15
                    }
                    Text {
                        Layout.preferredWidth: 100
                        Layout.rightMargin: 40
                        text: qsTr("歌手id")
                        font.pixelSize: 15
                    }
                }
                ListView {
                    id: listView
                    Layout.fillWidth: true
                    Layout.preferredHeight: parent.height-row.height
                    Layout.leftMargin: 5
                    clip: true
                    currentIndex:0
                    model: lyricListModel
                    delegate: lyricListDelagate
                    ScrollBar.vertical: ScrollBar {
                        width: 12
                        policy: ScrollBar.AlwaysOn
                    }
                }
            }
            ListModel{
                id:lyricListModel
            }
            Component {
                id:lyricListDelagate
                Rectangle {
                    radius: 4
                    width: listView.width
                    height: 40
                    focus: true
                    color:ListView.isCurrentItem ? "lightgrey" : "white"
                    RowLayout{
                        id:sarchLayout
                        Text {
                            text:song
                            Layout.preferredWidth:100
                            Layout.rightMargin: 40
                            elide: Text.ElideRight
                        }
                        Text {
                            text:artist_id
                            Layout.preferredWidth: 100
                            anchors.rightMargin: 40
                            elide: Text.ElideRight
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
                                 lyricDownload.onDoubleClick(listView.currentIndex)
                            }
                        }
                    }

                   Menu {
                        id: option_menu
                        x:mX
                        y:mY
                        contentData:[
                            download
                        ]
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
                   Menu{
                       id:menu1
                       x:mX
                       y:mY
                       contentData: [
                          download
                       ]
                  }
                }
            }
        }
    }
    onClosing: {
        lyricListModel.clear();
        searchList.visible=false
        keywordInput.clear()
    }
    LyricDownload{
        id:lyricDownload
        onSongNameChanged: {
             lyricListModel.clear();
            for(var i=0;i<songName.length;i++) {
                lyricListModel.append({"song":songName[i],"artist_id":artist_id[i]})
            }
        }
        onLyricChanged: {
            lyricDownloadDialog.open();
        }
        onShowLyricChanged: {
            lyricShowDialog.netLyric.text=lyricDownload.showLyric
            lyricShowDialog.visible=true
        }
    }
    FileDialog{
        id:lyricDownloadDialog
        title: "Please choose a file"
        folder:shortcuts.documents
        nameFilters: [ "Lrc Files(*.lrc *.txt)",
                        "*.*"]
        selectExisting: false
        onAccepted: {
            dialogs.lyricDialog.fileIo.source=lyricDownloadDialog.fileUrl.toString().slice(7)
            dialogs.lyricDialog.fileIo.write(dialogs.lyricSearchDialog.lyricDownload.lyric);
        }
    }
    MessageDialog{
        id:hashEmptyDialog
        title:"May I have your attention please"
        text:"please input the hash value and the keyword value and duration"
        standardButtons: StandardButton.Ok
    }

    LyricShowDialog{
        id:lyricShowDialog
        visible: false
    }

}

