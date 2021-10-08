#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H


#include <QWidget>
#include <QPushButton>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QFile>

class AudioRecorder : public QWidget
{
    Q_OBJECT

public:
    AudioRecorder(QWidget *parent = 0);
    Q_INVOKABLE void startRecord();
    Q_INVOKABLE void stopRecord();
    Q_INVOKABLE void play();
    Q_INVOKABLE void save(QString filename);
    Q_INVOKABLE void pause();

private:
    int AddWavHeader(char *);
    int ApplyVolumeToSample(short iSample);
    void InitMonitor();
    void CreateAudioInput();
    void CreateAudioOutput();


private:
    QAudioFormat mFormatFile;
    QFile *mpOutputFile;

    QAudioInput *mpAudioInputFile;		// 负责读写文件
    QAudioOutput *mpAudioOutputFile;

    QByteArray mBuffer;
};

#endif // AUDIORECORDER_H
