#include "audiorecorder.h"
#include <QLayout>
#include <QDebug>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>

#define BufferSize 14096

struct HEADER
{
    char RIFFNAME[4];       //资源交换文件标志RIFF
    unsigned nRIFFLength;      //从下个地址开始到文件尾的总字节数
    char WAVNAME[4];                //WAV文件标志
    char FMTNAME[4];                //波形格式(fmt),最后一位空格
    unsigned nFMTLength;       //过滤字节
    unsigned short nAudioFormat;    //格式种类
    unsigned short nChannleNumber;  //通道数
    unsigned nSampleRate;      //采样频率
    unsigned nBytesPerSecond;  //数据传输率
    unsigned short nBytesPerSample; //块对齐
    unsigned short nBitsPerSample;  //样本数据位数
    char    DATANAME[4];
    unsigned nDataLength;
};

AudioRecorder::AudioRecorder(QWidget *parent)
    : QWidget(parent)
    , mBuffer(BufferSize, 0)
{

    QDir dir("/tmp/KirinMusic");
    if(!dir.exists()){
        dir.mkdir("/tmp/KirinMusic");
    }

    mpOutputFile = NULL;
    mpAudioInputFile = NULL;
    mpAudioOutputFile = NULL;

    mpOutputFile = new QFile();
    mpOutputFile->setFileName(tr("/tmp/KirinMusic/record.raw"));

    mFormatFile.setSampleRate(44100);
    mFormatFile.setChannelCount(2);
    mFormatFile.setSampleSize(16);
    mFormatFile.setCodec("audio/pcm");
    mFormatFile.setSampleType(QAudioFormat::SignedInt);
    mFormatFile.setByteOrder(QAudioFormat::LittleEndian);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(mFormatFile)) {
        qWarning("input default mFormatFile not supported try to use nearest");
        mFormatFile = info.nearestFormat(mFormatFile);
    }

    QAudioDeviceInfo info1(QAudioDeviceInfo::defaultOutputDevice());
    if (!info1.isFormatSupported(mFormatFile)) {
        qWarning() << "output default mFormatFile not supported - trying to use nearest";
        //           mFormatFile = info.nearestFormat(mFormatSound);
        qWarning() << "output no support input mFormatFile.";
        return;
    }

    if(mFormatFile.sampleSize() != 16) {
        qWarning("audio device doesn't support 16 bit support %d bit samples, example cannot run", mFormatFile.sampleSize());
        mpAudioInputFile = 0;
        return;
    }

    mpAudioInputFile = NULL;
    mpAudioOutputFile = NULL;
}


void AudioRecorder::startRecord()
{
    mpOutputFile->open(QIODevice::WriteOnly | QIODevice::Truncate);

    mpAudioInputFile = new QAudioInput(mFormatFile);
    mpAudioInputFile->start(mpOutputFile);
    qDebug()<<"Start record";
}

void AudioRecorder::play()
{
    mpOutputFile->open(QIODevice::ReadOnly);

    mpAudioOutputFile = new QAudioOutput(mFormatFile);
    mpAudioOutputFile->start(mpOutputFile);
    qDebug()<<"Start play";
}

void AudioRecorder::stopRecord()
{
    mpAudioInputFile->stop();
    qDebug()<<"Stop record";
    mpOutputFile->close();
}

void AudioRecorder::save(QString filename)
{
    if(filename.length() == 0) {
        QMessageBox::information(NULL, tr("filename"), tr("You didn't select any files."));
    } else {
        if(AddWavHeader(filename.toLatin1().data())>0){
            QMessageBox::information(NULL, tr("Save"), tr("Success Save :") + filename);
        }else{
            QMessageBox::information(NULL, tr("Save"), tr("Fail Save :") + filename);
        }
    }
}

void AudioRecorder::pause()
{
    mpAudioOutputFile->stop();
    qDebug()<<"Stop play";
    mpOutputFile->close();
}

int AudioRecorder::AddWavHeader(char *filename)
{
    HEADER DestionFileHeader;
    DestionFileHeader.RIFFNAME[0] = 'R';    //资源文件标志
    DestionFileHeader.RIFFNAME[1] = 'I';
    DestionFileHeader.RIFFNAME[2] = 'F';
    DestionFileHeader.RIFFNAME[3] = 'F';

    DestionFileHeader.WAVNAME[0] = 'W';     //代表wav文件格式
    DestionFileHeader.WAVNAME[1] = 'A';
    DestionFileHeader.WAVNAME[2] = 'V';
    DestionFileHeader.WAVNAME[3] = 'E';

    DestionFileHeader.FMTNAME[0] = 'f';     //波形格式标志
    DestionFileHeader.FMTNAME[1] = 'm';
    DestionFileHeader.FMTNAME[2] = 't';
    DestionFileHeader.FMTNAME[3] = 0x20;    //最后需要一个空格
    DestionFileHeader.nFMTLength = 16;      //表示FMT的长度
    DestionFileHeader.nAudioFormat = 1;     //表示PCM编码

    DestionFileHeader.DATANAME[0] = 'd';
    DestionFileHeader.DATANAME[1] = 'a';
    DestionFileHeader.DATANAME[2] = 't';
    DestionFileHeader.DATANAME[3] = 'a';
    DestionFileHeader.nBitsPerSample = 16;
    DestionFileHeader.nBytesPerSample = 4;      //块对齐
    DestionFileHeader.nSampleRate = 44100;      //采样率
    DestionFileHeader.nBytesPerSecond = 176400; //Byte率=采样频率*音频通道数*每次采样得到的样本位数/8
    DestionFileHeader.nChannleNumber = 2;       //双声道
    int nFileLen = 0;
    int nSize = sizeof(DestionFileHeader);
    qDebug()<<nSize;

    FILE *fp_s = NULL;
    FILE *fp_d = NULL;

    fp_s = fopen("/tmp/KirinMusic/record.raw", "rb");
    if (fp_s == NULL){
        return -1;
    }

    fp_d = fopen(filename, "wb+");
    if (fp_d == NULL)
        return -2;

    int nWrite = fwrite(&DestionFileHeader, 1, nSize, fp_d);
    if (nWrite != nSize)
    {
        fclose(fp_s);
        fclose(fp_d);
        return -3;
    }

    while( !feof(fp_s))
    {
        char readBuf[4096];     //硬盘分区是以最小4096Byte为单位
        int nRead = fread(readBuf, 1, 4096, fp_s);
        if (nRead > 0)
        {
            fwrite(readBuf, 1, nRead, fp_d);
        }

        nFileLen += nRead;
    }
    fseek(fp_d, 0L, SEEK_SET);

    DestionFileHeader.nRIFFLength = nFileLen - 8 + nSize;
    DestionFileHeader.nDataLength = nFileLen;
//    qDebug()<<DestionFileHeader.nDataLength;
//    qDebug()<<DestionFileHeader.nRIFFLength;
    nWrite = fwrite(&DestionFileHeader, 1, nSize, fp_d);
    if (nWrite != nSize)
    {
        fclose(fp_s);
        fclose(fp_d);
        return -4;
    }

    fclose(fp_s);
    fclose(fp_d);
    return nFileLen;
}
