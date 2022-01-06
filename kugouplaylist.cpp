#include "kugouplaylist.h"
#include "kugousong.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>
#include <QMessageBox>
KuGouPlayList::KuGouPlayList(QObject *parent) : QObject(parent)
{
    network_manager = new QNetworkAccessManager();
    network_request = new QNetworkRequest();				//发送请求一得到AlbumID和FileHash
    network_manager2 = new QNetworkAccessManager();
    network_request2 = new QNetworkRequest();			//发送请求二歌单中的歌曲列表
    network_manager3 = new QNetworkAccessManager();
    network_request3 = new QNetworkRequest();
    network_manager4 = new QNetworkAccessManager();
    network_request4 = new QNetworkRequest();

    network_request2->setRawHeader("Cookie","kg_mid=233");
    network_request2->setHeader(QNetworkRequest::CookieHeader,"2333");
    network_request3->setRawHeader("Cookie","kg_mid=233");
    network_request3->setHeader(QNetworkRequest::CookieHeader,"2333");
    connect(network_manager4, &QNetworkAccessManager::finished, this, &KuGouPlayList::replyFinished4);
    connect(network_manager3, &QNetworkAccessManager::finished, this, &KuGouPlayList::replyFinished3);
    connect(network_manager2, &QNetworkAccessManager::finished, this, &KuGouPlayList::replyFinished2);
    connect(network_manager, &QNetworkAccessManager::finished, this, &KuGouPlayList::replyFinished);

}

void KuGouPlayList::searchPlayList(QString str)
{
    m_specialName.clear();
    specialId.clear();
    m_nickName.clear();
    m_playCount.clear();
    QString KGAPISTR1 = QString("http://mobilecdnbj.kugou.com/api/v3/search/special?version=9108&highlight=em&keyword=%1"
        "&pagesize=30&filter=0&page=1&sver=2&with_res_tag=1").arg(str);

    network_request->setUrl(QUrl(KGAPISTR1));
    network_manager->get(QNetworkRequest((*network_request)));
}

void KuGouPlayList::parseJson_getSpecialID(QString json)
{
    json=json.replace("<!--KG_TAG_RES_START-->","").replace("<!--KG_TAG_RES_END-->","");
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
                            int size = array.size();
                            for(int i = 0;i < size;i++)
                            {
                                QJsonValue value = array.at(i);
                                if(value.isObject())
                                {
                                    QJsonObject object = value.toObject();
                                    if(object.contains("specialid"))
                                    {
                                        QJsonValue specialid_value = object.take("specialid");
                                        if(specialid_value.isDouble())
                                        {
                                            long num=(long)specialid_value.toDouble();
                                            specialId<<num;
                                        }
                                    }

                                    if(object.contains("specialname"))
                                    {
                                        QJsonValue specialname_value = object.take("specialname");
                                        if(specialname_value.isString())
                                        {
                                            m_specialName<<specialname_value.toString();
                                        }
                                    }

                                    if(object.contains("nickname"))
                                    {
                                        QJsonValue nickname_value = object.take("nickname");
                                        if(nickname_value.isString())
                                        {
                                            m_nickName<<nickname_value.toString();
                                        }
                                    }

                                    if(object.contains("playcount"))
                                    {
                                        QJsonValue playcount_value = object.take("playcount");
                                        if(playcount_value.isDouble())
                                        {
                                            m_playCount<<playcount_value.toDouble();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        qDebug()<<json_error.errorString();
    }
}

void KuGouPlayList::parseJson_getPlayList(QString json)
{
    int n = json.indexOf("var data=");
    int m = json.indexOf("html\"}]");
    QString str=json.mid(n+10,m-n-4);

    int a=0,j=0;
    QString songinfo = str.toLower();
    QList<QJsonObject> playList;
    QList<QString> s;
    for(int pos=0;pos<songinfo.length();pos++) {
        if(songinfo[pos] == "{") {
            a=a+1;
        } else if(songinfo[pos] == "}") {
            a=a-1;
        } else {
            continue;
        }
        if (a == 0) {
            QString ch=songinfo.mid(j,pos+1-j);
            QJsonDocument jsonDocument = QJsonDocument::fromJson(ch.toLocal8Bit().data());
            if( jsonDocument.isNull() ){
                qDebug()<< "===> please check the string "<< ch.toLocal8Bit().data();
            }
            QJsonObject jsonObject = jsonDocument.object();
            playList<<jsonObject;
            pos += 1;
            j=pos+1;
        }
    }
    m_songName.clear();
    m_singerName.clear();
    m_albumName.clear();
    hash.clear();
    album_id.clear();
    for(int i=0;i<playList.length();i++) {
        QJsonValue value=playList[i];
        if(value.isObject())
        {
            QJsonObject object = value.toObject();
            if(object.contains("hash"))
            {
                QJsonValue hash_value = object.take("hash");
                if(hash_value.isString())
                {
                    hash<<hash_value.toString();
                }
            }
            if(object.contains("album_id"))
            {
                QJsonValue albumId_value = object.take("album_id");
                if(albumId_value.isDouble())
                {
                    album_id<<long(albumId_value.toDouble());
                }
            }

            if(object.contains("songname"))
            {
                QJsonValue songname_value = object.take("songname");
                if(songname_value.isString())
                {
                    m_songName<<songname_value.toString();
                }
            }
            if(object.contains("singername"))
            {
                QJsonValue singername_value = object.take("singername");
                if(singername_value.isString())
                {
                    m_singerName<<singername_value.toString();
                }
            }
            if(object.contains("album_name"))
            {
                QJsonValue albumname_value = object.take("album_name");
                if(albumname_value.isString())
                {
                    m_albumName<<albumname_value.toString();
                }
            }
        }
    }
}

void KuGouPlayList::parseJson_getPlayUrl(QString json)
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
                                qDebug()<<play_urlStr;
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

void KuGouPlayList::getSongList(int index)
{
    QString KGAPISTR1 = QString("https://www.kugou.com/yy/special/single/%1.html").arg(specialId[index]);
    network_request2->setUrl(QUrl(KGAPISTR1));
    network_manager2->get(QNetworkRequest((*network_request2)));
}

void KuGouPlayList::getSongUrl(int index)
{
    m_image.clear();
    m_url.clear();
    m_lyrics.clear();
    //通过歌曲ID发送请求，得到歌曲url和歌词
    QString KGAPISTR1 = QString("http://wwwapi.kugou.com/yy/index.php?r=play/getdata&hash=%1&album_id=%2").arg(hash[index]).arg(album_id[index]);
    network_request3->setUrl(QUrl(KGAPISTR1));
    network_manager3->get(*network_request3);
}

void KuGouPlayList::downloadSong(int index,QString path)
{
    m_savePath=path;
    isDownloadSong=true;
    getSongUrl(index);
    connect(this,&KuGouPlayList::getUrl,this,&KuGouPlayList::writeUrl);
}
void KuGouPlayList::writeUrl()
{
    network_request4->setUrl(QUrl(m_url));
    network_manager4->get(*network_request4);
}

void KuGouPlayList::replyFinished(QNetworkReply *reply)
{
    //获取响应的信息，状态码为200表示正常
   // QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
        parseJson_getSpecialID(result);  //自定义方法，解析歌曲数据
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }

    emit specialNameChanged();
    reply->deleteLater();
}

void KuGouPlayList::replyFinished2(QNetworkReply *reply)
{
    //获取响应的信息，状态码为200表示正常
   // QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
        parseJson_getPlayList(result);  //自定义方法，解析歌曲数据
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }
    reply->deleteLater();   //最后要释放reply对象
    emit songNameChanged();
}

void KuGouPlayList::replyFinished3(QNetworkReply *reply)
{
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
        emit urlChanged();
    } else {
        emit getUrl();
    }
    isDownloadSong=false;
    reply->deleteLater();   //最后要释放reply对象
}

void KuGouPlayList::replyFinished4(QNetworkReply *reply)
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



const QList<double> &KuGouPlayList::playCount() const
{
    return m_playCount;
}

void KuGouPlayList::setPlayCount(const QList<double> &newPlayCount)
{
    if (m_playCount == newPlayCount)
        return;
    m_playCount = newPlayCount;
    emit playCountChanged();
}

const QList<QString> &KuGouPlayList::specialName() const
{
    return m_specialName;
}

void KuGouPlayList::setSpecialName(const QList<QString> &newSpecialName)
{
    if (m_specialName == newSpecialName)
        return;
    m_specialName = newSpecialName;
    emit specialNameChanged();
}

const QList<QString> &KuGouPlayList::nickName() const
{
    return m_nickName;
}

void KuGouPlayList::setNickName(const QList<QString> &newNickName)
{
    if (m_nickName == newNickName)
        return;
    m_nickName = newNickName;
    emit nickNameChanged();
}

const QList<QString> &KuGouPlayList::songName() const
{
    return m_songName;
}

void KuGouPlayList::setSongName(const QList<QString> &newSongName)
{
    if (m_songName == newSongName)
        return;
    m_songName = newSongName;
    emit songNameChanged();
}

const QList<QString> &KuGouPlayList::singerName() const
{
    return m_singerName;
}

void KuGouPlayList::setSingerName(const QList<QString> &newSingerName)
{
    if (m_singerName == newSingerName)
        return;
    m_singerName = newSingerName;
    emit singerNameChanged();
}

const QList<QString> &KuGouPlayList::albumName() const
{
    return m_albumName;
}

void KuGouPlayList::setAlbumName(const QList<QString> &newAlbumName)
{
    if (m_albumName == newAlbumName)
        return;
    m_albumName = newAlbumName;
    emit albumNameChanged();
}

const QString &KuGouPlayList::url() const
{
    return m_url;
}

void KuGouPlayList::setUrl(const QString &newUrl)
{
    if (m_url == newUrl)
        return;
    m_url = newUrl;
    emit urlChanged();
}

const QString &KuGouPlayList::image() const
{
    return m_image;
}

void KuGouPlayList::setImage(const QString &newImage)
{
    if (m_image == newImage)
        return;
    m_image = newImage;
    emit imageChanged();
}

const QString &KuGouPlayList::lyrics() const
{
    return m_lyrics;
}

void KuGouPlayList::setLyrics(const QString &newLyrics)
{
    if (m_lyrics == newLyrics)
        return;
    m_lyrics = newLyrics;
    emit lyricsChanged();
}
