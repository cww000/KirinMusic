#ifndef LYRICDOWNLOAD_H
#define LYRICDOWNLOAD_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QList>
#include <QObject>
class LyricDownload : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QString> songName READ songName WRITE setSongName NOTIFY songNameChanged)
    Q_PROPERTY(QList<QString> id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QList<double> score READ score WRITE setScore NOTIFY scoreChanged)
    Q_PROPERTY(QList<double> duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(QList<QString> singerName READ singerName WRITE setSingerName NOTIFY singerNameChanged)

    Q_PROPERTY(QString  lyric READ lyric WRITE setLyric NOTIFY lyricChanged)
    Q_PROPERTY(QString  showLyric READ showLyric WRITE setShowLyric NOTIFY showLyricChanged)
public:
    explicit LyricDownload(QObject *parent = nullptr);
    void lyricSearch(QString hash);
    Q_INVOKABLE void getHash(QString keyword);
    void parseJson_getID(QString result);
    void parseJson_getLyrics(QString result);
    void parseJson_getHash(QString result);
    Q_INVOKABLE void onClickDownload(int index);
    Q_INVOKABLE void onDoubleClick(int index);
    QList<QString> songName() const
    {
        return m_songName;
    }


    QString lyric() const
    {
        return m_lyric;
    }

    QString showLyric() const
    {
        return m_showLyric;
    }

    const QList<QString> &id() const;
    void setId(const QList<QString> &newId);

    const QList<double> &score() const;
    void setScore(const QList<double> &newScore);

    const QList<double> &duration() const;
    void setDuration(const QList<double> &newDuration);

    const QList<QString> &singerName() const;
    void setSingerName(const QList<QString> &newSingerName);

public slots:


    void setSongName(QList<QString> songName)
    {
        if (m_songName == songName)
            return;

        m_songName = songName;
        emit songNameChanged(m_songName);
    }



    void setLyric(QString lyric)
    {
        if (m_lyric == lyric)
            return;

        m_lyric = lyric;
        emit lyricChanged(m_lyric);
    }

    void setShowLyric(QString showLyric)
    {
        if (m_showLyric == showLyric)
            return;

        m_showLyric = showLyric;
        emit showLyricChanged(m_showLyric);
    }

protected slots:
    void replyFinished(QNetworkReply *reply);
    void replyFinished2(QNetworkReply*reply);
    void replyFinished3(QNetworkReply*reply);
    void replyFinished4(QNetworkReply*reply);

signals:
    void songNameChanged(QList<QString> songName);

    void lyricChanged(QString lyric);

    void showLyricChanged(QString showLyric);

    void idChanged();

    void scoreChanged();

    void durationChanged();

    void singerNameChanged();

private:
    QNetworkAccessManager *network_manager;
    QNetworkAccessManager *network_manager2;
    QNetworkAccessManager *network_manager3;
    QNetworkAccessManager *network_manager4;
    QNetworkRequest *network_request;
    QNetworkRequest *network_request2;
    QNetworkRequest *network_request3;
    QNetworkRequest *network_request4;

    QString m_hash;
    QList<QString> m_songName;
    QList<QString> m_singerName;
    QList<double> m_duration;
    QList<double> m_score;
    QList<QString> m_id;
    QList<QString> m_accesskey;


    QString m_lyric;
    QString m_showLyric;
    QString m_netlyric;
};

#endif // LYRICDOWNLOAD_H
