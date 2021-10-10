import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import Song 1.0

ApplicationWindow{
    id: songTagWindow
    title: qsTr("曲目信息")
    width:680
    height:450
    visible: true
    property alias song: song
    property var map:{"标题":" ","艺术家":" ","唱片集":" ","注释":" ","日期":" ","音轨号":" ","流派":" "};
    property alias picImage: picImage
    property bool flag: false
    signal showImage

    onShowImage: {      //显示专辑封面
        var str
        if(!dialogs.songSearchDialog.networkPlay){
            str = content.musicPlayer.audio.source.toString().slice(7)
        }else{
            str = ""
        }
        dialogs.songTagDialog.song.getTags(str, dirPath)
        dialogs.songTagDialog.get_Tags_Meta()
        dialogs.songTagDialog.picImage.source = ""
        if(dialogs.songTagDialog.song.flag){
            rootImage.source = ""
            content.leftImage.source = ""
            rootImage.source = "file://"+dirPath+"/pic.png"
            content.leftImage.source = "file://"+dirPath+"/pic.png"
            dialogs.songTagDialog.picImage.source = "file://"+dirPath+"/pic.png"

        }else if(dialogs.songSearchDialog.networkPlay){
         //   console.log(true)
            dialogs.songTagDialog.picImage.source = ""
        }else{
            rootImage.source = imageUrl
            content.leftImage.source = imageUrl
        }
    }

    function get_Tags_Meta(){
        titleInput.text = "";
        artistInput.text = "";
        albumInput.text = "";
        annotationInput.text = "";
        yearInput.text = "";
        trackInput.text = "";
        generInput.editText = "";

        titleInput.text=song.Tags["标题"];
        artistInput.text=song.Tags["艺术家"];
        albumInput.text=song.Tags["唱片集"];
        annotationInput.text=song.Tags["注释"];
        if(song.Tags["日期"]===0) {
            yearInput.text="";
        } else {
            yearInput.text=song.Tags["日期"];
        }
        if(song.Tags["音轨号"]===0) {
            trackInput.text= "";
        } else {
            trackInput.text= song.Tags["音轨号"];
        }
        generInput.editText=song.Tags["流派"];
        flag =false
    }

    function set_Tag_Meta(){
        map["标题"]=titleInput.text;

        map["艺术家"]=artistInput.text;
        map["唱片集"]=albumInput.text;
        map["注释"]=annotationInput.text;
        if(yearInput.text==="") {
           map["日期"]=0;
        } else {
            map["日期"]=yearInput.text;
        }
        if(trackInput.text==="") {
           map["音轨号"]=0;
        } else {
            map["音轨号"]=trackInput.text;
        }
        console.log(map["音轨号"])
        map["流派"]=generInput.editText;
        var path = content.musicPlayer.audio.source.toString().slice(7)
        song.saveTags(path,map)
        flag = false
        songTagWindow.close()
    }

    ColumnLayout{
        anchors.fill: parent
        RowLayout{
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.height-bottomBtn.height-20
            spacing: 10
            GroupBox{
                implicitWidth: parent.width/2-10
                implicitHeight: parent.height
                ColumnLayout{
                    anchors.fill: parent
                    Text {
                        id: artText
                        font.pointSize: 12
                        text: qsTr("专辑封面")
                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    }
                    Image {
                        id: picImage
                        cache: false
                        Layout.preferredWidth: parent.width-40
                        Layout.preferredHeight: parent.height-100
                        Layout.alignment: Qt.AlignHCenter
                        fillMode: Image.PreserveAspectCrop
                    }
                }
            }
            GroupBox{
                implicitWidth: parent.width/2-10
                implicitHeight: parent.height
                ColumnLayout{
                    width: parent.width
                    spacing: 10
                    Text{
                        font.pointSize: 12
                        text: qsTr(" 音频信息")
                    }
                    ColumnLayout{
                        Layout.fillWidth: true
                        spacing: 10
                        RowLayout{
                            Layout.fillWidth: true
                            Text {
                                id:title
                                text: qsTr("标   题:")
                            }
                            TextField{
                                id: titleInput
                                Layout.fillWidth: true
                                focus: true
                                onTextChanged: {
                                    flag = true
                                }
                            }
                        }
                        RowLayout{
                            Layout.fillWidth: true
                            Text {
                                id:artist
                                text: qsTr("艺术家:")
                            }
                            TextField{
                                id: artistInput
                                Layout.fillWidth: true
                                onTextChanged: {
                                    flag = true
                                }
                            }
                        }
                        RowLayout{
                            Layout.fillWidth: true
                            Text {
                                id:album
                                text: qsTr("唱片集:")
                            }
                            TextField{
                                id: albumInput
                                Layout.fillWidth: true
                                onTextChanged: {
                                    flag = true
                                }
                            }
                        }
                        RowLayout{
                            Layout.fillWidth: true
                            RowLayout{
                                Layout.fillWidth: true
                                RowLayout{
                                    Layout.fillWidth: true
                                    Text {
                                        id:track
                                        text: qsTr("音轨号:")
                                    }
                                    TextField{
                                        id: trackInput
                                        Layout.fillWidth: true
                                        validator: RegularExpressionValidator{regularExpression: /[0-9]+/}
                                        onTextChanged: {
                                            flag = true
                                        }
                                    }
                                }
                                RowLayout{
                                    Layout.fillWidth: true
                                    Text {
                                        id:year
                                        text: qsTr("日 期:")
                                    }
                                    TextField{
                                        id: yearInput
                                        Layout.fillWidth: true
                                        validator: RegularExpressionValidator{regularExpression: /[0-9]+/}
                                        onTextChanged: {
                                            flag = true
                                        }
                                    }
                                }
                            }
                        }
                        RowLayout{
                            Layout.fillWidth: true
                            Text {
                                id: gener
                                text: qsTr("流 派：")
                            }
                            ComboBox{
                                Layout.fillWidth: true
                                id:generInput
                                editable: true
                                currentIndex: -1
                            //    highlighted: Rectangle{color: "lightblue";width: genreInput.width}
                                model:ListModel{
                                    id:genreModel
                                    ListElement{text:"Acousic"}
                                    ListElement{text:"Abstract"}
                                    ListElement{text:"A Cappella"}
                                    ListElement{text:"Acid"}
                                    ListElement{text :"Acid Jazz"}
                                }
                                onEditTextChanged: {
                                    flag = true
                                }
                            }
                        }
                        RowLayout{
                            Text {
                                id: annotation
                                text: qsTr("注 释：")
                            }
                            TextField{
                                id: annotationInput
                                Layout.fillWidth: true
                                onTextChanged: {
                                    flag = true
                                }
                            }
                        }
                    }
                }
            }
        }
        RowLayout{
            id:bottomBtn
            Layout.margins: 10
            spacing: parent.width-labelButton.width-fileLayout.width-20
            Button{
                id:labelButton
                text: "从文件名获取标签"
            }
            RowLayout{
                id:fileLayout
                Button{
                    text:"保存到文件"
                    onClicked: {
                        set_Tag_Meta();
                    }
                }
                Button{
                    text: "关闭"
                    onClicked: {
                        if(flag){
                            dialogs.saveDialog.open()
                        }else{
                            close()
                        }
                    }
                }
            }
        }
    }
    onClosing: function(closeevent){
        closeevent.accepted = false
        if(flag){
            dialogs.saveDialog.open()
        }else{
            closeevent.accepted = true
        }
    }
    Song{
        id: song
    }

}
