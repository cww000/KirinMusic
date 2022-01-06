#ifndef KUGOUMV_H
#define KUGOUMV_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QList>


class KuGouMv : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QString> mvName READ mvName WRITE setMvName NOTIFY mvNameChanged)
    Q_PROPERTY(QList<QString> singerName READ singerName WRITE setSingerName NOTIFY singerNameChanged)
    Q_PROPERTY(QList<double> duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(QString mvUrl READ mvUrl WRITE setMvUrl NOTIFY mvUrlChanged)
    Q_PROPERTY(QList<QString> mvPic READ mvPic WRITE setMvPic NOTIFY mvPicChanged)

public:
    explicit KuGouMv(QObject *parent = nullptr);
    Q_INVOKABLE void searchMv(QString str);
    void parseJson_getMvHash(QString json);
    void parseJson_getMvUrl(QString json);
    Q_INVOKABLE void getMvUrl(int index);
    Q_INVOKABLE void downloadMv(int index,QString path);

    const QList<QString> &mvName() const;
    void setMvName(const QList<QString> &newMvName);

    const QList<QString> &singerName() const;
    void setSingerName(const QList<QString> &newSingerName);

    const QList<double> &duration() const;
    void setDuration(const QList<double> &newDuration);

    const QString &mvUrl() const;
    void setMvUrl(const QString &newMvUrl);

    const QList<QString> &mvPic() const;
    void setMvPic(const QList<QString> &newMvPic);

protected slots:
    void replyFinished(QNetworkReply *reply);
    void replyFinished2(QNetworkReply *reply);
    void replyFinished3(QNetworkReply *reply);
    void writeMv();

signals:

    void mvNameChanged();

    void singerNameChanged();

    void durationChanged();

    void mvUrlChanged();

    void mvPicChanged();
    void getMv();

private:
    QNetworkAccessManager *network_manager;
    QNetworkAccessManager *network_manager2;
    QNetworkRequest *network_request;
    QNetworkRequest *network_request2;
    QNetworkAccessManager *network_manager3;
    QNetworkRequest *network_request3;
    QList<QString> mvHash;
    QList<QString> m_mvName;
    QList<QString> m_singerName;
    QList<double> m_duration;
    QString m_mvUrl;
    QList<QString> m_mvPic;
    bool isDownloadMv=false;
    QString m_savePath;

};

#endif // KUGOUMV_H
