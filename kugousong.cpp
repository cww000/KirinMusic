#include "kugousong.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPushButton>
#include <QFile>
KuGouSong::KuGouSong(QObject *parent) : QObject(parent)
{
    network_manager = new QNetworkAccessManager();
    network_request = new QNetworkRequest();				//发送请求一得到AlbumID和FileHash
    network_manager2 = new QNetworkAccessManager();
    network_request2 = new QNetworkRequest();			//发送请求二得到url和歌词等信息
    network_manager3 = new QNetworkAccessManager();
    network_request3 = new QNetworkRequest();

    network_request2->setRawHeader("Cookie","kg_mid=233");
    network_request2->setHeader(QNetworkRequest::CookieHeader,2333);
    connect(network_manager3, &QNetworkAccessManager::finished, this, &KuGouSong::replyFinished3);
    connect(network_manager2, &QNetworkAccessManager::finished, this, &KuGouSong::replyFinished2);
    connect(network_manager, &QNetworkAccessManager::finished, this, &KuGouSong::replyFinished);
}

void KuGouSong::searchSong(QString str)
{
    clear();    //清空所有容器
    //发送歌曲搜索请求
    QString KGAPISTR1 = QString("http://songsearch.kugou.com/song_search_v2?keyword=%1&page=&pagesize=40"
          "&userid=-1&clientver=&platform=WebFilter&tag=em&filter=2&iscorrection=1&privilege_filter=0").arg(str);

    network_request->setUrl(QUrl(KGAPISTR1));
    network_manager->get(QNetworkRequest((*network_request)));
}

void KuGouSong::parseJson_getAlbumID(QString json)
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
                    if(valuedataObject.contains("lists"))
                    {
                        QJsonValue valueArray = valuedataObject.value("lists");
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
                                    if(object.contains("AlbumID"))
                                    {
                                        QJsonValue AlbumID_value = object.take("AlbumID");
                                        if(AlbumID_value.isString())
                                        {
                                            album_idStr.push_back(AlbumID_value.toString());             //歌曲ID信息
                                        }
                                    }
                                    if(object.contains("FileHash"))
                                    {
                                        QJsonValue FileHash_value = object.take("FileHash");
                                        if(FileHash_value.isString())
                                        {
                                            hashStr.push_back(FileHash_value.toString());                //hash
                                        }
                                    }

                                    if(object.contains("SongName"))
                                    {
                                        QJsonValue SongName_value = object.take("SongName");
                                        if(SongName_value.isString())
                                        {
                                            m_songName.push_back(SongName_value.toString());
                                        }
                                    }
                                    if(object.contains("AlbumName"))
                                    {
                                        QJsonValue AlbumName_value = object.take("AlbumName");
                                        if(AlbumName_value.isString())
                                        {
                                            m_albumName.push_back(AlbumName_value.toString());
                                        }
                                    }
                                    if(object.contains("SingerName"))
                                    {
                                        QJsonValue SingerName_value = object.take("SingerName");
                                        if(SingerName_value.isString())
                                        {
                                            m_singerName.push_back(SingerName_value.toString());
                                        }
                                    }
                                    if(object.contains("Duration"))
                                    {
                                        QJsonValue Duration_value = object.take("Duration");
                                        if(Duration_value.isDouble())
                                        {
                                            m_duration.push_back(Duration_value.toDouble());
                                        }
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
}

void KuGouSong::parseJson_getPlayUrl(QString json)
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

                    if(valuedataObject.contains("lyrics"))                           //lrc
                    {
                        QJsonValue play_lyric_value = valuedataObject.take("lyrics");
                        if(play_lyric_value.isString())
                        {
                            QString play_lrcStr = play_lyric_value.toString();
                            if(play_lrcStr!="")
                            {
                                m_lyrics=play_lrcStr;

                            }
                        }
                    }
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
                    if(valuedataObject.contains("img"))
                    {
                        QJsonValue play_img_value = valuedataObject.take("img");
                        if(play_img_value.isString())
                        {
                            QString play_img = play_img_value.toString();                    //歌曲的url
                            if(play_img!="")
                            {
                                m_image=play_img;
                            }
                        }
                    }
                }
            }
        }
    }
}

void KuGouSong::getSongUrl(int index)
{
    //通过歌曲ID发送请求，得到歌曲url和歌词
    QString KGAPISTR1 = QString("http://wwwapi.kugou.com/yy/index.php?r=play/getdata&hash=%1&album_id=%2").arg(hashStr[index]).arg(album_idStr[index]);
    network_request2->setUrl(QUrl(KGAPISTR1));
    network_manager2->get(*network_request2);
}

void KuGouSong::downloadSong(int index,QString path)
{
    m_savePath=path;
    isDownloadSong=true;
    getSongUrl(index);
    connect(this,&KuGouSong::getUrl,this,&KuGouSong::writeUrl);
}

void KuGouSong::clear()
{
    m_singerName.clear();
    m_songName.clear();
    m_albumName.clear();
    hashStr.clear();
    album_idStr.clear();
    m_duration.clear();
    m_lyrics.clear();
    m_url.clear();
    m_image.clear();
}

void KuGouSong::replyFinished(QNetworkReply *reply)
{
    //获取响应的信息，状态码为200表示正常
   // QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
        parseJson_getAlbumID(result);  //自定义方法，解析歌曲数据
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }

    emit songNameChanged(m_songName);
    reply->deleteLater();
}

void KuGouSong::replyFinished2(QNetworkReply *reply)
{
    //获取响应的信息，状态码为200表示正常
   // QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
        parseJson_getPlayUrl(result);  //自定义方法，解析歌曲数据
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }
    if(!isDownloadSong) {
        emit urlChanged(m_url);
    } else {
        emit getUrl();
    }
    isDownloadSong=false;
    reply->deleteLater();   //最后要释放reply对象
    //   qDebug()<<m_lyrics;
}

void KuGouSong::replyFinished3(QNetworkReply *reply)
{
    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QFile file(m_savePath);
        bool ok=file.open(QIODevice::WriteOnly|QIODevice::Truncate);
        if(ok) {
            qDebug()<<"正在下载歌曲，请稍等.....";
            file.write(bytes);
            file.close();
         }
         qDebug()<<"下载成功";
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }
    reply->deleteLater();   //最后要释放reply对象
}

void KuGouSong::writeUrl()
{
    network_request3->setUrl(QUrl(m_url));
    network_manager3->get(*network_request3);
}

