import QtQuick 2.0
import QtQml.Models 2.15
import QtQuick.Controls 2.15
import Clipboard 1.0


Item {
    property alias lyricText:lyricText
    property alias lyricListView:lyricListView
    property alias lyricListModel:lyricListModel
    property alias lyricDelegate:lyricDelegate
    property alias clipBoard: clipBoard


    Text {
        id: lyricText
        text: qsTr("当前无歌词")
        font.pointSize: 20
        visible: false
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: 300

    }
    ListView{
        id:lyricListView
        model: lyricListModel
        delegate: lyricDelegate
        anchors.fill: parent
        visible: false
        spacing: 30
    }
    ListModel{
        id:lyricListModel
    }
    Component{
        id:lyricDelegate
        Text {
            id: lyricText
            text: currentLyrics
            width: 300
            font.pointSize: 15
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: ListView.isCurrentItem ? "red" : "black"
        }
    }
    Clipboard{
        id: clipBoard
    }
}
