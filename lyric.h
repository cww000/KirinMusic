/*
* author: 程炆炆，聂海艳，李纯林
* email：1460964870@qq.com 1933544851@qq.com 2742731130@qq.com
* time:2021.10

* Copyright (C) <2021>  <Wenwen Cheng,Haiyan Nie,Chunlin Li>

* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef LYRIC_H
#define LYRIC_H

#include <QObject>
#include <QDebug>
#include <QList>
class Lyric: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int highlightPos READ highlightPos WRITE sethighlightPos NOTIFY highlightPosChanged)
    Q_PROPERTY(int highlightLength READ highlightLength WRITE sethighlightLength NOTIFY highlightLengthChanged)
    Q_PROPERTY(QString lyric READ lyric WRITE setLyric NOTIFY lyricChanged)
    Q_PROPERTY(QList<int> timeStamp READ timeStamp WRITE setTimeStamp NOTIFY timeStampChanged)
    Q_PROPERTY(QList<QString> plainLyric READ plainLyric WRITE setPlainLyric NOTIFY plainLyricChanged)
    Q_PROPERTY(int timeDif READ timeDif WRITE setTimeDif NOTIFY timeDifChanged)
public:
    explicit Lyric(QObject *parent = nullptr);

    Q_INVOKABLE QString addTag(QString content,int pos,QString str);
    Q_INVOKABLE QString deleteHeaderLabel(QString content,int pos);
    Q_INVOKABLE QString deleteAllLabel(QString content,int pos);
    Q_INVOKABLE void test(QString time);
    Q_INVOKABLE void extract_timeStamp();  //提取时间戳
    Q_INVOKABLE int findTimeInterval(QString nowTime);
    Q_INVOKABLE QString translateStamp(int time);
    Q_INVOKABLE QString translateStamp1(int time);
    Q_INVOKABLE bool lrcFlag(){return m_lrcFlag;}

    int translate(QString time);  //将时间戳转换成毫秒数 00:00.00
    int translate1(QString time);      //  00:00
    void sort();                    //转换后的时间戳数组按升序排序
    int highlightPos() const
    {
        return m_highlightPos;
    }

    int highlightLength() const
    {
        return m_highlightLength;
    }

    QString lyric() const
    {
        return m_lyric;
    }


    QList<int> timeStamp() const
    {
        return m_timeStamp;
    }

    int timeDif() const
    {
        return m_timeDif;
    }

    QList<QString> plainLyric() const
    {
        return m_plainLyric;
    }

public slots:
    void sethighlightPos(int highlightPos)
    {
        if (m_highlightPos == highlightPos)
            return;

        m_highlightPos = highlightPos;
        emit highlightPosChanged(m_highlightPos);
    }

    void sethighlightLength(int highlightLength)
    {
        if (m_highlightLength == highlightLength)
            return;

        m_highlightLength = highlightLength;
        emit highlightLengthChanged(m_highlightLength);
    }

    void setLyric(QString lyric)
    {
        if (m_lyric == lyric)
            return;

        m_lyric = lyric;
        emit lyricChanged(m_lyric);
    }


    void setTimeStamp(QList<int> timeStamp)
    {
        if (m_timeStamp == timeStamp)
            return;

        m_timeStamp = timeStamp;
        emit timeStampChanged(m_timeStamp);
    }

    void setTimeDif(int timeDif)
    {
        if (m_timeDif == timeDif)
            return;

        m_timeDif = timeDif;
        emit timeDifChanged(m_timeDif);
    }

    void setPlainLyric(QList<QString> plainLyric)
    {
        if (m_plainLyric == plainLyric)
            return;

        m_plainLyric = plainLyric;
        emit plainLyricChanged(m_plainLyric);
    }

signals:
    void highlightPosChanged(int highlightPos);

    void highlightLengthChanged(int highlightLength);

    void lyricChanged(QString lyric);

    void timeStampChanged(QList<int> timeStamp);

    void timeDifChanged(int timeDif);

    void plainLyricChanged(QList<QString> plainLyric);

private:
     int topOfLine(QString content,int pos);
     bool isNum(char *ch);
     bool isEqual(QString str1,QString str2);

     QString m_lyric;
     QList<int> m_timeStamp;  //存放化为了毫秒的时间戳
     int m_highlightPos;
     int m_highlightLength;
     int m_timeDif;   //时间差
     QList<QString> m_plainLyric;
     bool m_lrcFlag;
};

#endif // LYRIC_H
