#include "karaoke.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Karaoke::Karaoke(QObject *parent) : QObject(parent)
{
    network_manager = new QNetworkAccessManager();
    network_request = new QNetworkRequest();				//发送请求一得到AlbumID和FileHash
    network_manager2 = new QNetworkAccessManager();
    network_request2 = new QNetworkRequest();			//发送请求二得到url和歌词等信息

    connect(network_manager2, &QNetworkAccessManager::finished, this, &Karaoke::replyFinished2);
    connect(network_manager, &QNetworkAccessManager::finished, this, &Karaoke::replyFinished);
}

void Karaoke::search(QString str)
{
    m_url.clear();
    hashStr.clear();
    album_idStr.clear();
    //发送歌曲搜索请求
    QString keyword=str+"伴奏";
QString KGAPISTR1=QString("http://mobilecdn.kugou.com/api/v3/search/song?format=json"
                          "&keyword=%1&page=%1&pagesize=30").arg(keyword);
    network_request->setUrl(QUrl(KGAPISTR1));
    network_manager->get(QNetworkRequest((*network_request)));
}

void Karaoke::replyFinished(QNetworkReply *reply)
{
    //获取响应的信息，状态码为200表示正常
 //   QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

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

void Karaoke::parseJson_getHash(QString json)     //解析接收到的歌曲信息，得到歌曲ID
{
    QByteArray ba=json.toUtf8();
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
                                        hashStr=FileHash_value.toString();                //hash
                                    }
                                }
                                if(object.contains("album_id"))
                                {
                                    QJsonValue AlbumID_value = object.take("album_id");
                                    if(AlbumID_value.isString())
                                    {
                                        album_idStr=AlbumID_value.toString();             //歌曲ID信息
                                    }
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

    //通过歌曲ID发送请求，得到音乐伴奏的url
    QString KGAPISTR1 = QString("https://www.kugou.com/yy/index.php?r=play/getdata&hash=%1&album_id=%2&_=1497972864535").arg(hashStr).arg(album_idStr);
    network_request2->setUrl(QUrl(KGAPISTR1));
    //不加头无法得到json，可能是为了防止机器爬取
    network_request2->setRawHeader("Cookie","kg_mid=233");
    network_request2->setHeader(QNetworkRequest::CookieHeader, 2333);
    network_manager2->get(*network_request2);
}

void Karaoke::replyFinished2(QNetworkReply *reply)
{
 //   QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符
        parseJson_getplay_url(result);  //自定义方法，解析歌曲数据

    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }

    emit urlChanged();
  //  qDebug()<<m_url<<"  "<<m_lyrics;

}

void Karaoke::parseJson_getplay_url(QString json)        //解析得到歌曲
{
    QByteArray ba=json.toUtf8();
    const char *ch=ba.data();

    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment =QJsonDocument::fromJson(byte_array.append(ch),&json_error);
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
                    if(valuedataObject.contains("play_url"))
                    {
                        QJsonValue play_url_value = valuedataObject.take("play_url");
                        if(play_url_value.isString())
                        {
                            QString play_urlStr = play_url_value.toString();                    //歌曲的url
                            if(play_urlStr!="")
                            {
                                m_url=play_urlStr;
                            }
                        }
                    }
                    if(valuedataObject.contains("lyrics"))
                    {
                        QJsonValue lyrics_value = valuedataObject.take("lyrics");
                        if(lyrics_value.isString())
                        {
                            QString lyricsStr = lyrics_value.toString();                    //歌曲的url
                            if(lyricsStr!="")
                            {
                                m_lyrics=lyricsStr;
                            }
                        }
                    }
                }
            }
        }
    }
}



const QString &Karaoke::lyrics() const
{
    return m_lyrics;
}

void Karaoke::setLyrics(const QString &newLyrics)
{
    if (m_lyrics == newLyrics)
        return;
    m_lyrics = newLyrics;
    emit lyricsChanged();
}
