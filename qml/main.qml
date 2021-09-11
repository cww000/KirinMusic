import QtQuick 2.0

Item{
    id:root
    width: 870
    height: 680
    visible: true

    MainWindow{
        id:appWindow
        width: root.width
        height: root.height
    }

    Karaoke{
        id:karaoke
        width: root.width
        height: root.height
    }

    Component.onCompleted: {
        loader.setSource("qrc:MainWindow.qml")
    }

    Loader{
        id:loader
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top:parent.top
        anchors.bottom: parent.bottom
    }
}
