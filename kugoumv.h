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

public:
    explicit KuGouMv(QObject *parent = nullptr);
    Q_INVOKABLE void searchMv(QString str);
    void parseJson_getMvHash(QString json);
    void parseJson_getMvUrl(QString json);
    Q_INVOKABLE void getMvUrl(int index);

    const QList<QString> &mvName() const;
    void setMvName(const QList<QString> &newMvName);

    const QList<QString> &singerName() const;
    void setSingerName(const QList<QString> &newSingerName);

    const QList<double> &duration() const;
    void setDuration(const QList<double> &newDuration);

    const QString &mvUrl() const;
    void setMvUrl(const QString &newMvUrl);

protected slots:
    void replyFinished(QNetworkReply *reply);
    void replyFinished2(QNetworkReply*reply);

signals:

    void mvNameChanged();

    void singerNameChanged();

    void durationChanged();

    void mvUrlChanged();

private:
    QNetworkAccessManager *network_manager;
    QNetworkAccessManager *network_manager2;
    QNetworkRequest *network_request;
    QNetworkRequest *network_request2;
    QList<QString> mvHash;
    QList<QString> m_mvName;
    QList<QString> m_singerName;
    QList<double> m_duration;
    QString m_mvUrl;

};

#endif // KUGOUMV_H
