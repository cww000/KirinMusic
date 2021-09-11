import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtMultimedia 5.8

Item{
    id:karaokeWindow
    width:870
    height:680
    visible: true
    property string  currentTime:content.musicPlayer.currentTime
    property string  totalTime:content.musicPlayer.totalTime

    ColumnLayout{
        spacing: 10
        Text {
            id: songName
            font.pointSize: 20
            text: content.musicPlayer.fileName
            Layout.leftMargin: (appWindow.width-songName.width)/2
        }
        Text {
            id: time
            text:"正在录制 "+currentTime+"/"+totalTime
            font.pointSize: 10
            Layout.leftMargin: (appWindow.width-time.width)/2
        }

        LyricPage{
            id:karaokeLyric
            visible:true
            Layout.topMargin: 15
            width: 300
            height: 400
        }

        RowLayout{
            Layout.leftMargin: (appWindow.width-songName.width)/2.3
            Layout.topMargin: 10
            spacing: 20

            Image{
                id:goBack
                source: "qrc:/image/返回.png"
                Layout.preferredWidth: 35
                Layout.preferredHeight: 35
                TapHandler{
                    onTapped: {
                        content.visible=true
                        menuBar.visible=true
                        karaoke.visible=false
                        appWindow.title="KirinMusic"
                    }
                }
            }

            Image{
                id:pauseButton
                Layout.preferredWidth: 35
                Layout.preferredHeight: 35
                source: "qrc:/image/开始录制.png"
                visible: false
                TapHandler{
                    onTapped: {
                        recordButton.visible=true
                        pauseButton.visible=false
                    }
                }
            }

            Image{
                id:recordButton
                Layout.preferredWidth: 35
                Layout.preferredHeight: 35
                source: "qrc:/image/红点.png"
                visible: true
                TapHandler{
                    onTapped: {
                        recordButton.visible=false
                        pauseButton.visible=true
                    }
                }
            }

            Image{
                id:saveButton
                Layout.preferredWidth: 35
                Layout.preferredHeight: 35
                source: "qrc:/image/保存.png"
            }
        }
    }

    Audio{
        id:karaokeAudio

    }
}
