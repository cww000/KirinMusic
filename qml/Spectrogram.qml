import QtQuick 2.2
import QtQuick.Controls 1.1
import Decode 1.0

Canvas{
    id:canvasLine
    property alias decode: decode
    property alias speTimer: timer
    property int lineStart: 0
    property int lineEnd: 0
    property int pcount: 0
    property double yend: 0.0
    property var vertices:[]

    function getVertices(){
        vertices = []
        var str = content.musicPlayer.audio.source.toString().slice(7)
        content.spectrogram.decode.decode(str, dirPath)      //得到绘制顶点
        vertices = decode.vertices
        pcount = 0
    }
    function canvasClear(){
        var ctx = getContext("2d");
        ctx.clearRect(0, 0, canvasLine.width, canvasLine.height)
        ctx.stroke();
    }

    onPaint: {
        var ctx = getContext("2d");
        ctx.clearRect(0, 0, canvasLine.width, canvasLine.height)
        ctx.lineWidth=3;
        ctx.strokeStyle = "Teal";
        ctx.beginPath();
        for(var i = lineStart; i < lineEnd; i++){
            yend = 110-vertices[i];
            ctx.moveTo((i-lineStart+1)*10,yend);
            ctx.lineTo((i-lineStart+1)*10, 110);

        }
        ctx.stroke();
    }
    Timer{
        id: timer
        interval: 100;
        running: false;
        repeat: true
        onTriggered: {
            lineStart = pcount*34
            pcount++;
            lineEnd = pcount*34
            if(lineEnd > vertices.length-1){
                stop()
            }
            canvasLine.requestPaint()
        }
    }
    Decode{
        id: decode
    }
}
