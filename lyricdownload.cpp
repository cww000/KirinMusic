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
    network_request3=new QNetworkRequest();                 //发送请求三得到url和歌词等信息
    network_manager2 = new QNetworkAccessManager();
    network_manager3 = new QNetworkAccessManager();
     connect(network_manager, &QNetworkAccessManager::finished, this, &LyricDownload::replyFinished);
    connect(network_manager3, &QNetworkAccessManager::finished, this, &LyricDownload::replyFinished3);
     connect(network_manager2, &QNetworkAccessManager::finished, this, &LyricDownload::replyFinished2);
}


void LyricDownload::lyricSearch(QString keyword)
{
    m_artist_id.clear();
    m_songName.clear();
    m_url.clear();
    QString KGAPISTR1;
    KGAPISTR1 = QString("http://gecimi.com/api/lyric/"+keyword);
    network_request->setUrl(QUrl(KGAPISTR1));
    network_manager->get(QNetworkRequest((*network_request)));

}

void LyricDownload::parseJson_getID(QString json)
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
            if(rootObj.contains("result"))
            {
                QJsonValue valueArray = rootObj.value("result");

                if (valueArray.isArray())
                {
                    QJsonArray array = valueArray.toArray();
                    int size = array.size();
                    for(int i = 0;i < size;i++)
                    {
                        QJsonValue value = array.at(i);
                        if(value.isObject())
                        {
                            QJsonObject object = value.toObject();
                            if(object.contains("artist_id"))
                            {
                                QJsonValue ID_value = object.take("artist_id");
                                if(ID_value.isDouble())
                                {
                                    m_artist_id<<ID_value.toDouble();             //歌词ID信息
                                }
                            }
                            if(object.contains("lrc"))
                            {
                                QJsonValue lrc_value = object.take("lrc");
                                if(lrc_value.isString())
                                {
                                    m_url<<lrc_value.toString();
                                }
                            }

                            if(object.contains("song"))
                            {
                                QJsonValue song_value = object.take("song");
                                if(song_value.isString())
                                {

                                   m_songName<<song_value.toString();             //歌词ID信息
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
    network_request2->setUrl(QUrl(m_url[index]));
    network_manager2->get(QNetworkRequest((*network_request2)));

}

void LyricDownload::onDoubleClick(int index)
{
    m_showLyric.clear();
    network_request3->setUrl(QUrl(m_url[index]));
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
        parseJson_getID(result);  //自定义方法，解析歌曲数据
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }
    emit songNameChanged(m_songName);
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
        m_lyric=result;
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

void LyricDownload::replyFinished3(QNetworkReply *reply)
{
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
        m_showLyric=result;
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

