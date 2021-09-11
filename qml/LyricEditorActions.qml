import QtQuick 2.12
import QtQuick.Controls 2.5
Item {
    property alias newAction: newA
    property alias openAction: openLyric
    property alias saveAction: save
    property alias closeAction: close
    property alias exitAction: exit
    property alias copyAction: copy
    property alias pasteAction: paste
    property alias cutAction: cut
    property alias undoAction: undo
    property alias redoAction: redo
    property alias addTagAction: addTag
    property alias deleteHeaderLabelAction:deleteHeaderLabel
    property alias deleteAllLabelAction:deleteAllLabel
    property alias testAction: test

    Action{
       id:newA
       text: qsTr("新建")
       shortcut: StandardKey.New
       icon.source: "qrc:/image/new.png"
    }


    Action{
        id:openLyric
        text: qsTr("打开文件")
        shortcut: StandardKey.Open
        icon.source: "qrc:/image/open.png"
    }
    Action{
        id:save
        text: qsTr("保存")
        shortcut: StandardKey.Save
        icon.source: "qrc:/image/save.png"
    }
    Action{
        id:close
        text: qsTr("关闭")
       shortcut: StandardKey.Close
       icon.source: "qrc:/image/close.png"
       enabled: textArea.text.length === 0 ? false : true
    }
    Action{
        id:exit
        text: qsTr("退出")
        shortcut: StandardKey.Quit
        icon.source: "qrc:/image/exit.png"
    }
    Action{
        id:copy
        text: qsTr("复制")
        shortcut: StandardKey.Copy
        icon.source: "qrc:/image/copy.png"
    }
    Action{
        id:paste
        text: qsTr("粘贴")
        shortcut: StandardKey.Paste
        icon.source: "qrc:/image/paste.png"
    }
    Action{
        id:cut
        text: qsTr("剪切")
        shortcut: StandardKey.Cut
        icon.source: "qrc:/image/cut.png"
    }
    Action{
        id:undo
        text:qsTr("撤销")
        shortcut: StandardKey.Undo
        icon.source: "qrc:/image/undo.png"
    }

    Action{
        id:redo
        text:qsTr("重做")
        shortcut: StandardKey.Redo
        icon.source: "qrc:/image/redo.png"
    }


    Action{
        id:addTag
        text: qsTr("加入标签")
        icon.source: "qrc:/image/add.png"
    }
    Action{
        id:deleteHeaderLabel
        text: qsTr("删除行内首标签")
        icon.source: "qrc:/image/delete.png"
    }
    Action{
        id:deleteAllLabel
        text: qsTr("删除行内所有标签")
    }
    Action{
        id:test
        text: qsTr("测试")
        icon.source: "qrc:/image/test.png"
        enabled: content.musicPlayer.pause.visible & textArea.length!==0
    }
}
