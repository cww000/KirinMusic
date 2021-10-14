import QtQuick 2.12
import QtQuick.Layouts 1.0
import QtQml.Models 2.15
import QtQuick.Controls 2.15
import Qt.labs.folderlistmodel 2.12
Item {
    width: 300
    height: 500
    property real  mX:0.0
    property real  mY:0.0
    property alias songListModel:songListModel
    property alias songListView:songListView
    property alias songDelegate:songDelegate


    Rectangle{
        anchors.fill: parent
        opacity: 0.6
        ListView{
            id:songListView
            anchors.fill: parent
            delegate: songDelegate
            model: songListModel
        }
        ListModel{
            id:songListModel
        }
        Component{
            id:songDelegate
            Rectangle{
                width: 300
                height: 40
                color:ListView.isCurrentItem ? "lightgrey" : "white"
                Text {
                    id: serialNumberText
                    text: index+1
                    font.pointSize: 15
                    width: 40
                    color: "Teal"
                    anchors.rightMargin: 40
                }
                Text {
                    id: chapterText
                    text: chapter
                    color: "Teal"
                    elide: Text.ElideRight
                    width: 220
                    anchors.left: serialNumberText.right
                    font.pointSize: 15
                }
                TapHandler{
                    acceptedButtons: Qt.RightButton //点击右键，content 响应右键的上下文菜单
                    onTapped: {
                        mX=eventPoint.position.x
                        mY=eventPoint.position.y
                        menu1.open()
                    }
                }

                TapHandler{
                    acceptedButtons:Qt.LeftButton
                    onTapped: {
                        dialogs.songSearchDialog.close()
                        content.musicPlayer.play(index)
                    }
                }
                Menu{
                    id:menu1
                    x:mX
                    y:mY
                    contentData: [
                        actions.pauseAction,
                        actions.deleteAction
                    ]
                }
            }
        }
    }
}
