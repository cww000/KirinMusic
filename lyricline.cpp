#include "lyricline.h"

LyricLine::LyricLine()
{

}

void LyricLine::parseKrcLyric()
{
    QString lyric=m_lineLyric;
    m_wordDuration.clear();
    QString str="";
    for(int i=0;i<lyric.length();i++) {
        if(lyric[i]==">") {
            str+=lyric[i+1];
        }
        if(lyric[i]=="[" && lyric[i+1].isDigit()) {
            for(int j=i;j<lyric.length();j++) {
                if(lyric[j]==",") {
                    QString start=lyric.mid(i+1,j-i-1);
                    m_startTime=start.toInt();                 //得到行开始时间
                    for(int h=j;h<lyric.length();h++) {
                        if(lyric[h]=="]") {
                            QString end=lyric.mid(j+1,h-j-1);
                            m_lineDuration=end.toInt();                 //得到行结束时间
                            break;
                        }
                    }
                    break;
                }
            }
        }
        if(lyric[i]=="<" && lyric[i+1].isDigit()) {
            for(int j=i;j<lyric.length();j++) {
                if(lyric[j]=="," && lyric[j+1].isDigit()) {
                    for(int h=j;h<lyric.length();h++) {
                        if(lyric[h]=="," && lyric[h+1]=="0") {
                            QString duration=lyric.mid(j+1,h-j-1);
                            m_wordDuration<<duration.toInt();     //歌词中每个字的持续时间
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    m_plainLyric=str;
}
