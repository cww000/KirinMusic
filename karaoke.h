#ifndef KARAOKE_H
#define KARAOKE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class Karaoke : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString lyrics READ lyrics WRITE setLyrics NOTIFY lyricsChanged)
public:
    explicit Karaoke(QObject *parent = nullptr);
    Q_INVOKABLE void search(QString keyword);
    void parseJson_getHash(QString json);
    void parseJson_getplay_url(QString json);


    const QString &url() const
    {
        return m_url;
    }

    const QString &lyrics() const;
    void setLyrics(const QString &newLyrics);

signals:
    void urlChanged();

    void lyricsChanged();

public slots:
    void setUrl(const QString &newUrl)
    {
        if (m_url == newUrl)
            return;
        m_url = newUrl;
        emit urlChanged();
    }

protected slots:
    void replyFinished(QNetworkReply*reply);
    void replyFinished2(QNetworkReply*reply);


private:
    QNetworkAccessManager *network_manager;
    QNetworkAccessManager *network_manager2;
    QNetworkRequest *network_request;
    QNetworkRequest *network_request2;
    QString album_idStr;
    QString hashStr;
    QString m_url;
    QString m_lyrics;

};

#endif // KARAOKE_H
