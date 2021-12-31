#include "kugoumv.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
KuGouMv::KuGouMv(QObject *parent) : QObject(parent)
{
    network_manager = new QNetworkAccessManager();
    network_request = new QNetworkRequest();				//发送请求一得到AlbumID和FileHash
    network_manager2 = new QNetworkAccessManager();
    network_request2 = new QNetworkRequest();			//发送请求二得到url和歌词等信息

    network_request2->setRawHeader("Cookie","kg_mid=233");
    network_request2->setHeader(QNetworkRequest::CookieHeader,2333);
    connect(network_manager2, &QNetworkAccessManager::finished, this, &KuGouMv::replyFinished2);
    connect(network_manager, &QNetworkAccessManager::finished, this, &KuGouMv::replyFinished);

}

void KuGouMv::searchMv(QString str)
{
    mvHash.clear();
    m_mvName.clear();
    m_singerName.clear();
    m_duration.clear();

    QString KGAPISTR1 = QString("http://mvsearch.kugou.com/mv_search?keyword=%1&page=1&pagesize=50&userid=-1"
          "&clientver=&platform=WebFilter&tag=em&filter=2&iscorrection=1&privilege_filter=0&_=1515052279917").arg(str);

    network_request->setUrl(QUrl(KGAPISTR1));
    network_manager->get(QNetworkRequest((*network_request)));
}

void KuGouMv::parseJson_getMvHash(QString json)
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
                                    if(object.contains("MvName"))
                                    {
                                        QJsonValue MvName_value = object.take("MvName");
                                        if(MvName_value.isString())
                                        {
                                            m_mvName<<MvName_value.toString();
                                        }
                                    }

                                    if(object.contains("Duration"))
                                    {
                                        QJsonValue Duration_value = object.take("Duration");
                                        if(Duration_value.isDouble())
                                        {
                                            m_duration<<Duration_value.toDouble();
                                        }
                                    }
                                    if(object.contains("SingerName"))
                                    {
                                        QJsonValue SingerName_value = object.take("SingerName");
                                        if(SingerName_value.isString())
                                        {
                                            m_singerName<<SingerName_value.toString();
                                        }
                                    }

                                    if(object.contains("MvHash"))
                                    {
                                        QJsonValue MvHash_value = object.take("MvHash");
                                        if(MvHash_value.isString())
                                        {
                                            mvHash<<MvHash_value.toString();
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

void KuGouMv::parseJson_getMvUrl(QString json)
{
    QByteArray ba=json.toUtf8();
    const char *ch=ba.data();

    QString rq="",sq="",le="";
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(ch), &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject rootObj = parse_doucment.object();
            if(rootObj.contains("mvdata"))
            {
                QJsonValue valuedata = rootObj.value("mvdata");
                if(valuedata.isObject())
                {
                    QJsonObject object = valuedata.toObject();
                    if(object.contains("sq"))
                    {
                         QJsonValue sqValue = object.value("sq");
                         if(sqValue.isObject())
                         {
                             QJsonObject sqObject=sqValue.toObject();
                             if(sqObject.contains("downurl"))
                             {
                                 QJsonValue mvurl_value = sqObject.take("downurl");
                                 if(mvurl_value.isString())
                                 {
                                     sq=mvurl_value.toString();
                                 }
                             }
                         }
                    }

                    if(object.contains("rq"))
                    {
                         QJsonValue rqValue = object.value("rq");
                         if(rqValue.isObject())
                         {
                             QJsonObject rqObject=rqValue.toObject();
                             if(rqObject.contains("downurl"))
                             {
                                 QJsonValue mvurl_value = rqObject.take("downurl");
                                 if(mvurl_value.isString())
                                 {
                                     rq=mvurl_value.toString();
                                 }
                             }
                         }
                    }

                    if(object.contains("le"))
                    {
                         QJsonValue leValue = object.value("le");
                         if(leValue.isObject())
                         {
                             QJsonObject leObject=leValue.toObject();
                             if(leObject.contains("downurl"))
                             {
                                 QJsonValue mvurl_value = leObject.take("downurl");
                                 if(mvurl_value.isString())
                                 {
                                     le=mvurl_value.toString();
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

    if(sq.length()==0) {
        if(rq.length()==0) {
            if(le.length()==0) {
                m_mvUrl="";
            } else {
                m_mvUrl=le;
            }
        } else {
            m_mvUrl=rq;
        }
    } else {
        m_mvUrl=sq;
    }
    emit mvUrlChanged();
}

void KuGouMv::getMvUrl(int index)
{
    m_mvUrl.clear();
    //通过mv hash值发送请求，得到mv url
    QString KGAPISTR1 = QString("http://m.kugou.com/app/i/mv.php?cmd=100&hash=%1&ismp3=1&ext=mp4").arg(mvHash[index]);
    network_request2->setUrl(QUrl(KGAPISTR1));
    network_manager2->get(QNetworkRequest((*network_request2)));

}

const QList<QString> &KuGouMv::mvName() const
{
    return m_mvName;
}

void KuGouMv::setMvName(const QList<QString> &newMvName)
{
    if (m_mvName == newMvName)
        return;
    m_mvName = newMvName;
    emit mvNameChanged();
}

const QList<QString> &KuGouMv::singerName() const
{
    return m_singerName;
}

void KuGouMv::setSingerName(const QList<QString> &newSingerName)
{
    if (m_singerName == newSingerName)
        return;
    m_singerName = newSingerName;
    emit singerNameChanged();
}

const QList<double> &KuGouMv::duration() const
{
    return m_duration;
}

void KuGouMv::setDuration(const QList<double> &newDuration)
{
    if (m_duration == newDuration)
        return;
    m_duration = newDuration;
    emit durationChanged();
}

const QString &KuGouMv::mvUrl() const
{
    return m_mvUrl;
}

void KuGouMv::setMvUrl(const QString &newMvUrl)
{
    if (m_mvUrl == newMvUrl)
        return;
    m_mvUrl = newMvUrl;
    emit mvUrlChanged();
}

void KuGouMv::replyFinished(QNetworkReply *reply)
{
    //获取响应的信息，状态码为200表示正常
   // QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
        parseJson_getMvHash(result);  //自定义方法，解析歌曲数据
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }

    emit mvNameChanged();
    reply->deleteLater();
}

void KuGouMv::replyFinished2(QNetworkReply *reply)
{
    //获取响应的信息，状态码为200表示正常
   // QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
        parseJson_getMvUrl(result);  //自定义方法，解析歌曲数据
    }
    else
    {
        //处理错误
        qDebug()<<"处理错误";
    }
    reply->deleteLater();   //最后要释放reply对象
 //   qDebug()<<m_lyrics;
}