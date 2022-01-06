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
#ifndef LYRICLINE_H
#define LYRICLINE_H

#include <QList>

class LyricLine
{
public:
    LyricLine();
    void parseKrcLyric();
    void setLyricLine(QString lyric) {m_lineLyric=lyric;}
    QString lyricLine() {return m_lineLyric;}
    int startTime() {return m_startTime;}
    int lineDuration() {return m_lineDuration;}
    QList<int> wordDuration() {return m_wordDuration;}
    QString plainLyric() {return m_plainLyric;}

private:
    QString m_lineLyric;
    QString m_plainLyric;      //每行纯歌词
    int  m_startTime;              //行开始时间
    int  m_lineDuration;                //行持续时间
    QList<int> m_wordDuration;                //行歌词中每个字的持续时间
};

#endif // LYRICLINE_H
