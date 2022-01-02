#ifndef KUGOUPLAYLIST_H
#define KUGOUPLAYLIST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QList>

class KuGouSong;
class KuGouPlayList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<double> playCount READ playCount WRITE setPlayCount NOTIFY playCountChanged)
    Q_PROPERTY(QList<QString> specialName READ specialName WRITE setSpecialName NOTIFY specialNameChanged)
    Q_PROPERTY(QList<QString> nickName READ nickName WRITE setNickName NOTIFY nickNameChanged)

public:
    explicit KuGouPlayList(QObject *parent = nullptr);
    Q_INVOKABLE void searchPlayList(QString str);
    void parseJson_getSpecialID(QString json);
    void parseJson_getPlayList(QString json);
    Q_INVOKABLE void getSongList(int index);
    const QList<double> &playCount() const;
    void setPlayCount(const QList<double> &newPlayCount);

    const QList<QString> &specialName() const;
    void setSpecialName(const QList<QString> &newSpecialName);

    const QList<QString> &nickName() const;
    void setNickName(const QList<QString> &newNickName);

signals:

    void playCountChanged();

    void specialNameChanged();

    void nickNameChanged();

protected slots:
    void replyFinished(QNetworkReply *reply);
    void replyFinished2(QNetworkReply*reply);

private:
    QNetworkAccessManager *network_manager;
    QNetworkAccessManager *network_manager2;
    QNetworkRequest *network_request;
    QNetworkRequest *network_request2;
    QList<long> specialId;
    QList<QString> m_specialName;
    QList<QString> m_nickName;
    QList<double> m_playCount;
    KuGouSong *m_kuGouSong;
};

#endif // KUGOUPLAYLIST_H
