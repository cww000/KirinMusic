#ifndef KUGOU_H
#define KUGOU_H
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QList>
#include <QObject>

class KuGou : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QString> singerName READ singerName WRITE setSingerName NOTIFY singerNameChanged)
    Q_PROPERTY(QList<QString> songName READ songName WRITE setSongName NOTIFY songNameChanged)
    Q_PROPERTY(QList<QString> albumName READ albumName WRITE setAlbumName NOTIFY albumNameChanged)
    Q_PROPERTY(QList<double> duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString lyrics READ lyrics WRITE setLyrics NOTIFY lyricsChanged)
    Q_PROPERTY(QString image READ image WRITE setImage NOTIFY imageChanged)
public:
    explicit KuGou(QObject *parent = nullptr);
    Q_INVOKABLE void search(QString str);
    void parseJson_getAlbumID(QString json);
    void parseJson_getplay_url(QString json);
    Q_INVOKABLE void onclickPlay(int index);

    QList<QString> singerName() const
    {
        return m_singerName;
    }

    QList<QString> songName() const
    {
        return m_songName;
    }

    QList<QString> albumName() const
    {
        return m_albumName;
    }

    QString url() const
    {
        return m_url;
    }

    QString lyrics() const
    {
        return m_lyrics;
    }



    QList<double> duration() const
    {
        return m_duration;
    }

    QString image() const
    {
        return m_image;
    }

public slots:
    void setSingerName(QList<QString> singerName)
    {
        if (m_singerName == singerName)
            return;

        m_singerName = singerName;
        emit singerNameChanged(m_singerName);
    }

    void setSongName(QList<QString> songName)
    {
        if (m_songName == songName)
            return;

        m_songName = songName;
        emit songNameChanged(m_songName);
    }

    void setAlbumName(QList<QString> albumName)
    {
        if (m_albumName == albumName)
            return;

        m_albumName = albumName;
        emit albumNameChanged(m_albumName);
    }

    void setUrl(QString url)
    {
        if (m_url == url)
            return;

        m_url = url;
        emit urlChanged(m_url);
    }

    void setLyrics(QString lyrics)
    {
        if (m_lyrics == lyrics)
            return;

        m_lyrics = lyrics;
        emit lyricsChanged(m_lyrics);
    }



    void setDuration(QList<double> duration)
    {
        if (m_duration == duration)
            return;

        m_duration = duration;
        emit durationChanged(m_duration);
    }

    void setImage(QString image)
    {
        if (m_image == image)
            return;

        m_image = image;
        emit imageChanged(m_image);
    }

protected slots:
    void replyFinished(QNetworkReply*reply);
    void replyFinished2(QNetworkReply*reply);

signals:
    void mediaAdd(QString play_urlStr);
    void nameAdd(QString play_name);
    void lrcAdd(QString play_lrcStr);

    void singerNameChanged(QList<QString> singerName);

    void songNameChanged(QList<QString> songName);

    void albumNameChanged(QList<QString> albumName);

    void urlChanged(QString url);

    void lyricsChanged(QString lyrics);

    void durationChanged(QList<double> duration);

    void imageChanged(QString image);

private:
    QNetworkAccessManager *network_manager;
    QNetworkAccessManager *network_manager2;
    QNetworkRequest *network_request;
    QNetworkRequest *network_request2;
    QList<QString> album_idStr;
    QList<QString> hashStr;
    QList<QString> m_albumName;
    QList<QString> m_songName;
    QList<QString> m_singerName;
    QList<double> m_duration;
    QString m_image;
    QString m_lyrics;
    QString m_url;

};

#endif // KUGOU_H
