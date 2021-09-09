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
    Q_PROPERTY(QList<double> artist_id READ artist_id WRITE setArtist_id NOTIFY artist_idChanged)
    Q_PROPERTY(QList<QString> songName READ songName WRITE setSongName NOTIFY songNameChanged)
    Q_PROPERTY(QString  lyric READ lyric WRITE setLyric NOTIFY lyricChanged)
    Q_PROPERTY(QString  showLyric READ showLyric WRITE setShowLyric NOTIFY showLyricChanged)
public:
    explicit LyricDownload(QObject *parent = nullptr);
    Q_INVOKABLE void lyricSearch(QString keyword);
    void parseJson_getID(QString result);
    void parseJson_getLyrics(QString result);
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



    QList<double> artist_id() const
    {
        return m_artist_id;
    }

    QString showLyric() const
    {
        return m_showLyric;
    }

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



    void setArtist_id(QList<double> artist_id)
    {
        if (m_artist_id == artist_id)
            return;

        m_artist_id = artist_id;
        emit artist_idChanged(m_artist_id);
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

signals:


    void songNameChanged(QList<QString> songName);

    void lyricChanged(QString lyric);



    void artist_idChanged(QList<double> artist_id);

    void showLyricChanged(QString showLyric);

private:
    QNetworkAccessManager *network_manager;
    QNetworkAccessManager *network_manager2;
    QNetworkAccessManager *network_manager3;
    QNetworkRequest *network_request;
    QNetworkRequest *network_request2;
    QNetworkRequest *network_request3;
    QList<double> m_artist_id;
    QList<QString> m_songName;
    QList<QString> m_url;
    QString m_lyric;
    QString m_showLyric;
};

#endif // LYRICDOWNLOAD_H
