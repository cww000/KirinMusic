#include "kugouplaylist.h"
#include "kugousong.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
KuGouPlayList::KuGouPlayList(QObject *parent) : QObject(parent),m_kuGouSong{new KuGouSong}
{
    network_manager = new QNetworkAccessManager();
    network_request = new QNetworkRequest();				//发送请求一得到AlbumID和FileHash
    network_manager2 = new QNetworkAccessManager();
    network_request2 = new QNetworkRequest();			//发送请求二得到url和歌词等信息

    network_request2->setRawHeader("Cookie","kg_mid=233");
    network_request2->setHeader(QNetworkRequest::CookieHeader,2333);
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
        }
    } else {
        qDebug()<<json_error.errorString();
    }
}

void KuGouPlayList::parseJson_getPlayList(QString json)
{
    qDebug()<<json;
}

void KuGouPlayList::getSongList(int index)
{
    m_kuGouSong->clear();
    QString KGAPISTR1 = QString("https://www.kugou.com/yy/special/single/%1.html").arg(specialId[index]);
    qDebug()<<KGAPISTR1;
    network_request2->setUrl(QUrl(KGAPISTR1));
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
 //   qDebug()<<m_lyrics;
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
