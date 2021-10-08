import QtQuick 2.0
import QtQml.Models 2.15
import QtQuick.Controls 2.15
import Clipboard 1.0
import QtGraphicalEffects 1.0


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
        currentIndex: -1

        //固定currentItem的位置
        highlightRangeMode:ListView.ApplyRange
        preferredHighlightBegin:height/3+30
        preferredHighlightEnd: height/3+60
    }
    ListModel{
        id:lyricListModel
    }
    Component{
        id:lyricDelegate
        Text {
            id: mt
            text: currentLyrics
            width: 300
            font.pixelSize: ListView.isCurrentItem ? 25 : 18
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: ListView.isCurrentItem ? "red":"black"
        }
    }
    Clipboard{
        id: clipBoard
    }
}
