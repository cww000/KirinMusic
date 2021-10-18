import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import Qt.labs.folderlistmodel 2.12
 import QtGraphicalEffects 1.15

ApplicationWindow{
    id:skinDialog
    title: qsTr("主题")
    width: 720
    height: 560

    property url usingImage:""

    ColumnLayout{
        Rectangle{
            width: 720
            height: 500
            ColumnLayout{
                anchors.fill: parent
                spacing: 10
                Text {
                    text: qsTr("正在使用")
                    Layout.topMargin: 10
                    Layout.leftMargin: 10
                }
                Image {
                    id: image
                    Layout.preferredWidth: 150
                    Layout.preferredHeight: 120
                    Layout.leftMargin: 20
                    fillMode: Image.PreserveAspectCrop
                    asynchronous: true
                    source: usingImage
                }
                Text {
                    text: qsTr("推荐")
                    Layout.topMargin: 10
                    Layout.leftMargin: 10
                }
                GridView{
                    id: imageView
                    width: 720
                    height: 280
                    Layout.topMargin: 10
                    Layout.leftMargin: 20
                    model: imageModel
                    delegate: imageDelegate
                    cellWidth: 170
                    cellHeight: 140
                    currentIndex: 0
                    highlightFollowsCurrentItem: false
                    highlight: Rectangle{
                        width: 155; height: 125
                        border.color: "blue"
                        border.width: 3
                        x: imageView.currentItem.x-2
                        y: imageView.currentItem.y-2
                    }
                }
            }
        }

        RowLayout{
            id: cde
            Layout.leftMargin: 20
            Layout.bottomMargin: 10
            spacing:540
            Button{
                text: "自定义"
                anchors.leftMargin: 10
                onClicked: {
                    dialogs.fileImageDialog.open()
                }
            }
            Button{
                text:"确定"
                onClicked: {
                    dialogs.lyricDialog.fileIo.saveBackgroundUrl(imageUrl)
                    usingImage = imageUrl
                    close()
                }
            }
        }
    }

    FolderListModel{
        id: imageModel
        folder: "qrc:/background"
    }

    Component{
        id:imageDelegate
        Image {
            id: image
            width: 150
            height: 120
            fillMode: Image.PreserveAspectCrop
            asynchronous: true
            source: fileUrl
            focus: true
            MouseArea{
                id:mouseArea
                anchors.fill:parent
                onClicked: {
                    imageView.currentIndex = index
                    imageUrl = fileUrl
                    dialogs.songTagDialog.showImage()
                }
            }
        }
    }

}
