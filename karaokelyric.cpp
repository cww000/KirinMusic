#include "karaokelyric.h"
#include "lyricline.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>
#include "zlib.h"
#include <QDir>
#include <QByteArray>

const int Keys[] = {64, 71, 97, 119, 94, 50, 116, 71, 81, 54, 49, 45, 206, 210,110, 105};

#define   CONVERT_SUCCESS       0x00
#define   CONVERT_FILE_EMPTY    0x01
#define   CONVERT_PARA_ERR      0x02
#define   CONVERT_FORMAT_ERR    0x03
#define   CONVERT_UNKNOWN       0x04
#define   MaxBufferSize         21504

KaraokeLyric::KaraokeLyric(QObject *parent) : QObject(parent),m_lyricLine{new LyricLine}
{
    network_manager = new QNetworkAccessManager();
    network_request = new QNetworkRequest();				//发送请求一得到AlbumID和FileHash
    network_manager2 = new QNetworkAccessManager();
    network_request2 = new QNetworkRequest();			//发送请求二得到url和歌词等信息

//    network_manager3 = new QNetworkAccessManager();
//    network_request3 = new QNetworkRequest();

    connect(network_manager, &QNetworkAccessManager::finished, this, &KaraokeLyric::replyFinished);
    connect(network_manager2, &QNetworkAccessManager::finished, this, &KaraokeLyric::replyFinished2);
}

KaraokeLyric::~KaraokeLyric()
{
    delete m_lyricLine;
}

void KaraokeLyric::lyricSearch()
{
    QString KGAPISTR1 = QString("http://krcs.kugou.com/search?ver=1&man=yes&client=mobi&duration=&hash=%1&album_audio_id=").arg(m_hash);
    network_request->setUrl(QUrl(KGAPISTR1));
    network_manager->get(QNetworkRequest((*network_request)));
}

int KaraokeLyric::findTimeInterval(QString nowTime)
{
    //先把这个时间戳转化为毫秒，然后在时间戳数组中找到第一个比他大的时间，两个时间相减得到时间差，返回给qml
    QString nowTime1=nowTime.mid(1,5);
    int time=translate(nowTime1);
    int interval=0,index=0;
    QList<int>::const_iterator constIterator;
    for (constIterator = m_startTime.constBegin(); constIterator != m_startTime.constEnd();
           ++constIterator) {
//        index++;
       if(time<=(*constIterator)) {
           interval=(*constIterator)-time;
            break;
       }
       index++;
    }
    m_timeDif=interval;
    return index;
}

void KaraokeLyric::parseJson_getID(QString result)
{
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(result), &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject rootObj = parse_doucment.object();
            if(rootObj.contains("candidates"))
            {
                QJsonValue valueArray = rootObj.value("candidates");

                if (valueArray.isArray())
                {
                    QJsonArray array = valueArray.toArray();
                    QJsonValue value = array.at(0);
                    if(value.isObject())
                    {
                        QJsonObject object = value.toObject();
                        if(object.contains("id"))
                        {
                            QJsonValue ID_value = object.take("id");
                            if(ID_value.isString())
                            {
                                id=ID_value.toString();             //歌词ID信息
                            }
                        }
                        if(object.contains("accesskey"))
                        {
                            QJsonValue accesskey_value = object.take("accesskey");
                            if(accesskey_value.isString())
                            {
                                accesskey=accesskey_value.toString();             //歌词ID信息
                            }
                        }

                        if(object.contains("duration"))
                        {
                            QJsonValue duration_value = object.take("duration");
                            if(duration_value.isDouble())
                            {
                               m_duration=duration_value.toDouble();
                            }
                        }

                        if(object.contains("score"))
                        {
                            QJsonValue score_value = object.take("score");
                            if(score_value.isDouble())
                            {
                               m_score=score_value.toDouble();
                            }
                        }

                      //  qDebug()<<m_duration<<" "<<m_score;
                        QString KGAPISTR2 = QString("http://lyrics.kugou.com/download?ver=1&client=pc&id=%1&accesskey=%2&fmt=krc&charset=utf8").arg(id).arg(accesskey);
                        network_request2->setUrl(QUrl(KGAPISTR2));
                        network_manager2->get(QNetworkRequest((*network_request2)));
                    }

                }
            }
        }
    }
    else
    {
        qDebug()<<json_error.errorString();
    }
}

void KaraokeLyric::parseJson_getLyrics(QString result)
{
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(result), &json_error);
    if(parse_doucment.isObject())
    {
        QJsonObject rootObj = parse_doucment.object();
        if(rootObj.contains("content"))
        {
            QJsonValue lyric_value = rootObj.value("content");
            if(lyric_value.isString())
            {
                //先将抓取到的歌词进行base64解码
                const char *s;
                QByteArray s1=lyric_value.toString().toLatin1();
                s=s1.data();
                QByteArray ba(s);
                QByteArray s2=QByteArray::fromBase64(ba);
//                qDebug()<<s2<<"\n";
                krcDecode(s2);
            }
        }
    }
}

void KaraokeLyric::getLyric()
{
    lyric.clear();
    QFile LrcFile("/tmp/KirinMusic/b.lrc");
    if(LrcFile.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QTextStream in(&LrcFile);
        while (!in.atEnd()) {
            QString str=in.readLine();
            if(str[0]=="[") {
                lyric<<str;
            } else {
                break;
            }
        }
    }

    LrcFile.close();

    //解析得到的KRC歌词
    m_plainLyric.clear();
    m_startTime.clear();
    m_lineDuration.clear();
    m_wordDuration.clear();
    for(int i=0;i<lyric.length();i++) {
       QString str=lyric[i];
       if(str[0]=="[" && str[1].isDigit()) {
           m_lyricLine->setLyricLine(lyric[i]);
           m_lyricLine->parseKrcLyric();

           m_startTime<<m_lyricLine->startTime();
           m_lineDuration<<m_lyricLine->lineDuration();
           m_plainLyric<<m_lyricLine->plainLyric();
           m_wordDuration<<m_lyricLine->wordDuration();
       }
    }

    emit plainLyricChanged();
//    for(int i=0;i<m_durations.length();i++) {
//         qDebug()<<m_plainLyric[i];
//         qDebug()<<m_startTime[i];
//     }


}

CONVERT_CODE KaraokeLyric::krcDecode(QByteArray krcData)
{
    QDir dir("/tmp/KirinMusic");
    if(!dir.exists()) {
        dir.mkdir("/tmp/KirinMusic");
    }
    //将数据解密解压缩后放到b.lrc中
    QFile LrcFile("/tmp/KirinMusic/b.lrc");
    CONVERT_CODE  nRet = CONVERT_PARA_ERR;
    if (LrcFile.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        QByteArray  DecodeData;
        if (!krcData.isEmpty())
        {
            // 校验开头4字符是否为正确
            if (krcData.left(4) == "krc1")
            {
                krcData.remove(0, 4);  // 去除文件头标识
                // XOR 大法解码
                for (int i = 0; i < krcData.size(); i++)
                {
                    DecodeData.append((char)(krcData[i] ^ Keys[i % 16]));
                }

                unsigned char *buffer_uncompress = new unsigned char[MaxBufferSize];
                unsigned long len_uncompress=MaxBufferSize;

                // 解压缩数据
                if(uncompress(buffer_uncompress,&len_uncompress,(Bytef*)DecodeData.data(),DecodeData.length())!=-1)
                {
                    char *str=(char *)buffer_uncompress;
                    //没办法准确获知解压后的数据大小
                    QByteArray bytearrary=QByteArray::fromRawData(str,MaxBufferSize);
                    QTextStream out(&LrcFile);
                    out<<bytearrary;
                    nRet = CONVERT_SUCCESS;
                }
                else
                {
                    nRet = CONVERT_FORMAT_ERR;
                }
            }
            else
            {
                nRet = CONVERT_FORMAT_ERR;
            }
        }
        else
        {
            nRet = CONVERT_FILE_EMPTY;
        }
    }
    LrcFile.close();

    getLyric();
    return nRet;
}

int KaraokeLyric::translate(QString time)
{
    int pos=0;
    int m_ms=0,s_ms=0;
    if(time.midRef(pos,1)=="0" && time.midRef(pos+1,1)=="0") {  //如果分钟数为0
        m_ms=0;
        if(time.midRef(pos+3,1)=="0" && time.midRef(pos+4,1)=="0") {   //如果秒数为0
            s_ms=0;
            return m_ms+s_ms;
        } else {
            s_ms=time.midRef(pos+3,2).toInt()*1000;
            return m_ms+s_ms;
        }
    } else {
         m_ms=time.midRef(pos,2).toInt()*60000;
         if(time.midRef(pos+3,1)=="0" && time.midRef(pos+4,1)=="0") {   //如果秒数为0
             s_ms=0;
             return m_ms+s_ms;
         } else {
             s_ms=time.midRef(pos+3,2).toInt()*1000;
             return m_ms+s_ms;
         }
    }
}

void KaraokeLyric::replyFinished(QNetworkReply *reply)
{
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
     //   qDebug()<<result;
        parseJson_getID(result);  //自定义方法，解析歌曲数据
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }
}

void KaraokeLyric::replyFinished2(QNetworkReply *reply)
{
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
        parseJson_getLyrics(result);  //自定义方法，解析歌曲数据
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }
}

const QString &KaraokeLyric::hash() const
{
    return m_hash;
}

void KaraokeLyric::setHash(const QString &newHash)
{
    if (m_hash == newHash)
        return;
    m_hash = newHash;
    emit hashChanged();
}

const QList<QString> &KaraokeLyric::plainLyric() const
{
    return m_plainLyric;
}

void KaraokeLyric::setPlainLyric(const QList<QString> &newPlainLyric)
{
    if (m_plainLyric == newPlainLyric)
        return;
    m_plainLyric = newPlainLyric;
 //   emit plainLyricChanged();
}

const QList<int> &KaraokeLyric::startTime() const
{
    return m_startTime;
}

void KaraokeLyric::setStartTime(const QList<int> &newStartTime)
{
    if (m_startTime == newStartTime)
        return;
    m_startTime = newStartTime;
    emit startTimeChanged();
}

const QList<int> &KaraokeLyric::lineDuration() const
{
    return m_lineDuration;
}

void KaraokeLyric::setLineDuration(const QList<int> &newLineDuration)
{
    if (m_lineDuration == newLineDuration)
        return;
    m_lineDuration = newLineDuration;
    emit lineDurationChanged();
}

int KaraokeLyric::timeDif() const
{
    return m_timeDif;
}

void KaraokeLyric::setTimeDif(int newTimeDif)
{
    if (m_timeDif == newTimeDif)
        return;
    m_timeDif = newTimeDif;
    emit timeDifChanged();
}
