/*
* author: 程炆炆，聂海艳，李纯林
* email：1460964870@qq.com 1933544851@qq.com 2742731130@qq.com
* time:2021.10
*
* Copyright (C) <2021>  <Wenwen Cheng,Haiyan Nie,Chunlin Li>
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
    Q_INVOKABLE void startRecord(QString dirPath);
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
    QString filePath;
};

#endif // AUDIORECORDER_H
