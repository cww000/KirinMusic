import QtQuick 2.0
import QtQuick.Dialogs 1.3

Item {
    property alias fileDialog: fileDialog
    property alias newDialog: newDialog
    property alias closeDialog: closeDialog

    FileDialog{
        id:fileDialog
        title: "Please choose a file"
        folder:shortcuts.documents
        nameFilters: [ "Lrc Files(*.lrc *.txt)",
                        "*.*"]
        selectExisting: false
    }
    MessageDialog{
        id: newDialog
        title: qsTr("create a new text")
        text: qsTr("文档已被修改\n您想要保存还是忽略更改?\n")
        standardButtons: StandardButton.Save|StandardButton.Discard|StandardButton.Cancel
        onButtonClicked: {
            if(clickedButton === StandardButton.Save){
                actions.saveAction.trigger()
                if(fileDialog.selectExisting === true){
                    textArea.text = ""
                    fileDialog.selectExisting = false
                }
            }else if(clickedButton === StandardButton.Discard){
                textArea.text= ""
            }else{
                close()
            }

            if(testNum===1) {
                action.testAction.triggered()
            }
        }
    }
    MessageDialog{
        id: closeDialog
        title: qsTr("create a new text")
        text: qsTr("文档已被修改\n您想要保存还是忽略更改?\n")
        standardButtons: StandardButton.Save|StandardButton.Discard|StandardButton.Cancel
        onButtonClicked: {
            if(clickedButton === StandardButton.Save){
                actions.saveAction.trigger()
                if(dialogs.fileDialog.selectExisting === true){
                    textArea.text = ""
                    dialogs.fileDialog.selectExisting = false
                }
            }else if(clickedButton === StandardButton.Discard){
                fileDialog.selectExisting = true;
                fileDialog.open();
            }else{
                close()
            }

            if(testNum===1) {
                action.testAction.triggered()
            }
        }
    }
}
