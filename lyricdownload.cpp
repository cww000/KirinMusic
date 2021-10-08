#include "lyricdownload.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
LyricDownload::LyricDownload(QObject *parent)
    : QObject(parent)
{
    network_manager = new QNetworkAccessManager();
    network_request = new QNetworkRequest();				//发送请求一得到AlbumID和FileHash

    network_request2= new QNetworkRequest();                //发送请求二得到url和歌词等信息
    network_manager2 = new QNetworkAccessManager();

    network_request3=new QNetworkRequest();                 //发送请求三得到url和歌词等信息
    network_manager3 = new QNetworkAccessManager();

    network_request4= new QNetworkRequest();
    network_manager4 = new QNetworkAccessManager();


    connect(network_manager, &QNetworkAccessManager::finished, this, &LyricDownload::replyFinished);
    connect(network_manager3, &QNetworkAccessManager::finished, this, &LyricDownload::replyFinished3);
    connect(network_manager2, &QNetworkAccessManager::finished, this, &LyricDownload::replyFinished2);
    connect(network_manager4, &QNetworkAccessManager::finished, this, &LyricDownload::replyFinished4);
}


void LyricDownload::lyricSearch(QString hash)
{
    m_singerName.clear();
    m_songName.clear();
    m_accesskey.clear();
    m_id.clear();
    m_score.clear();
    m_duration.clear();

    QString KGAPISTR1 = QString("http://krcs.kugou.com/search?ver=1&man=yes&client=mobi&duration=&hash=%1&album_audio_id=").arg(hash);
    network_request->setUrl(QUrl(KGAPISTR1));
    network_manager->get(QNetworkRequest((*network_request)));

}

void LyricDownload::getHash(QString keyword)
{
    QString KGAPISTR1=QString("http://mobilecdn.kugou.com/api/v3/search/song?format=json"
                          "&keyword=%1&page=1&pagesize=30").arg(keyword);
    network_request4->setUrl(QUrl(KGAPISTR1));
    network_manager4->get(QNetworkRequest((*network_request4)));
}

void LyricDownload::parseJson_getID(QString json)
{
    QByteArray byte_array;
    QJsonParseError json_error;
    QString text;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
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
                    for(int i=0;i<array.size();i++) {
                        QJsonValue value = array.at(i);
                        if(value.isObject())
                        {
                            QJsonObject object = value.toObject();
                            if(object.contains("id"))
                            {
                                QJsonValue ID_value = object.take("id");
                                if(ID_value.isString())
                                {
                                    m_id<<ID_value.toString();             //歌词ID信息
                                }
                            }
                            if(object.contains("accesskey"))
                            {
                                QJsonValue accesskey_value = object.take("accesskey");
                                if(accesskey_value.isString())
                                {
                                    m_accesskey<<accesskey_value.toString();             //歌词ID信息
                                }
                            }

                            if(object.contains("duration"))
                            {
                                QJsonValue duration_value = object.take("duration");
                                if(duration_value.isDouble())
                                {
                                   m_duration<<duration_value.toDouble();
                                }
                            }

                            if(object.contains("score"))
                            {
                                QJsonValue score_value = object.take("score");
                                if(score_value.isDouble())
                                {
                                   m_score<<score_value.toDouble();
                                }
                            }
                            if(object.contains("singer"))
                            {
                                QJsonValue singer_value = object.take("singer");
                                if(singer_value.isString())
                                {
                                   m_singerName<<singer_value.toString();
                                }
                            }

                            if(object.contains("song"))
                            {
                                QJsonValue song_value = object.take("song");
                                if(song_value.isString())
                                {
                                   m_songName<<song_value.toString();
                                }
                            }

                        }
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


void LyricDownload::onClickDownload(int index)
{
    m_lyric.clear();
    QString KGAPISTR2 = QString("http://lyrics.kugou.com/download?ver=1&client=pc&id=%1&accesskey=%2&fmt=lrc&charset=utf8").arg(m_id[index]).arg(m_accesskey[index]);
    network_request2->setUrl(QUrl(KGAPISTR2));
    network_manager2->get(QNetworkRequest((*network_request2)));

}

void LyricDownload::onDoubleClick(int index)
{
    m_showLyric.clear();
    QString KGAPISTR2 = QString("http://lyrics.kugou.com/download?ver=1&client=pc&id=%1&accesskey=%2&fmt=lrc&charset=utf8").arg(m_id[index]).arg(m_accesskey[index]);
    network_request3->setUrl(QUrl(KGAPISTR2));
    network_manager3->get(QNetworkRequest((*network_request3)));

}

void LyricDownload::replyFinished(QNetworkReply *reply)
{
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
      //  qDebug()<<result;
        parseJson_getID(result);  //自定义方法，解析歌曲数据
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }
    emit singerNameChanged();
    reply->deleteLater();
    reply=Q_NULLPTR;
}

void LyricDownload::replyFinished2(QNetworkReply *reply)
{
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
        parseJson_getLyrics(result);  //自定义方法，解析歌曲数据
        m_lyric=m_netlyric;
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }
    emit lyricChanged(m_lyric);
    reply->deleteLater();
    reply=Q_NULLPTR;
}

void LyricDownload::parseJson_getLyrics(QString json)
{
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
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
                QString s3(s2);
                m_netlyric=s3;
            }
        }
    }
}

void LyricDownload::replyFinished3(QNetworkReply *reply)
{
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
        parseJson_getLyrics(result);  //自定义方法，解析歌曲数据
        m_showLyric=m_netlyric;
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }
    emit showLyricChanged(m_showLyric);
    reply->deleteLater();
    reply=Q_NULLPTR;
}

void LyricDownload::replyFinished4(QNetworkReply *reply)
{
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
        parseJson_getHash(result);  //自定义方法，解析歌曲数据
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }
}

void LyricDownload::parseJson_getHash(QString result)
{
    QByteArray ba=result.toUtf8();
    const char *ch=ba.data();

    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(ch), &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject rootObj = parse_doucment.object();
            if(rootObj.contains("data"))
            {
                QJsonValue valuedata = rootObj.value("data");
                if(valuedata.isObject())
                {
                    QJsonObject valuedataObject = valuedata.toObject();
                    if(valuedataObject.contains("info"))
                    {
                        QJsonValue valueArray = valuedataObject.value("info");
                        if (valueArray.isArray())
                        {
                            QJsonArray array = valueArray.toArray();
                            QJsonValue value = array.at(0);
                            if(value.isObject())
                            {
                                QJsonObject object = value.toObject();
                                if(object.contains("hash"))
                                {
                                    QJsonValue FileHash_value = object.take("hash");
                                    if(FileHash_value.isString())
                                    {
                                        m_hash=FileHash_value.toString();                //hash
                                    }
                                    lyricSearch(m_hash);
                                }
                            }
                        }
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

const QList<QString> &LyricDownload::id() const
{
    return m_id;
}

void LyricDownload::setId(const QList<QString> &newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

const QList<double> &LyricDownload::score() const
{
    return m_score;
}

void LyricDownload::setScore(const QList<double> &newScore)
{
    if (m_score == newScore)
        return;
    m_score = newScore;
    emit scoreChanged();
}

const QList<double> &LyricDownload::duration() const
{
    return m_duration;
}

void LyricDownload::setDuration(const QList<double> &newDuration)
{
    if (m_duration == newDuration)
        return;
    m_duration = newDuration;
    emit durationChanged();
}

const QList<QString> &LyricDownload::singerName() const
{
    return m_singerName;
}

void LyricDownload::setSingerName(const QList<QString> &newSingerName)
{
    if (m_singerName == newSingerName)
        return;
    m_singerName = newSingerName;
    emit singerNameChanged();
}
