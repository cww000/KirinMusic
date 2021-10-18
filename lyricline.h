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
    QString m_plainLyric;      //每行歌词
    int  m_startTime;              //行开始时间
    int  m_lineDuration;                //行结束时间
    QList<int> m_wordDuration;                //行歌词中每个字的持续时间
};

#endif // LYRICLINE_H
