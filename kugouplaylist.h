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
    Q_PROPERTY(QList<QString> songName READ songName WRITE setSongName NOTIFY songNameChanged)
    Q_PROPERTY(QList<QString> singerName READ singerName WRITE setSingerName NOTIFY singerNameChanged)
    Q_PROPERTY(QList<QString> albumName READ albumName WRITE setAlbumName NOTIFY albumNameChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(QString lyrics READ lyrics WRITE setLyrics NOTIFY lyricsChanged)

public:
    explicit KuGouPlayList(QObject *parent = nullptr);
    Q_INVOKABLE void searchPlayList(QString str);
    void parseJson_getSpecialID(QString json);
    void parseJson_getPlayList(QString json);
    void parseJson_getPlayUrl(QString json);
    Q_INVOKABLE void getSongList(int index);
    Q_INVOKABLE void getSongUrl(int index);
    Q_INVOKABLE void downloadSong(int index,QString path);
    const QList<double> &playCount() const;
    void setPlayCount(const QList<double> &newPlayCount);

    const QList<QString> &specialName() const;
    void setSpecialName(const QList<QString> &newSpecialName);

    const QList<QString> &nickName() const;
    void setNickName(const QList<QString> &newNickName);

    const QList<QString> &songName() const;
    void setSongName(const QList<QString> &newSongName);

    const QList<QString> &singerName() const;
    void setSingerName(const QList<QString> &newSingerName);

    const QList<QString> &albumName() const;
    void setAlbumName(const QList<QString> &newAlbumName);

    const QString &url() const;
    void setUrl(const QString &newUrl);

    const QString &image() const;
    void setImage(const QString &newImage);

    const QString &lyrics() const;
    void setLyrics(const QString &newLyrics);

signals:

    void playCountChanged();

    void specialNameChanged();

    void nickNameChanged();

    void songNameChanged();

    void singerNameChanged();

    void albumNameChanged();

    void urlChanged();

    void imageChanged();

    void lyricsChanged();
    void getUrl();

protected slots:
    void replyFinished(QNetworkReply *reply);
    void replyFinished2(QNetworkReply*reply);
    void replyFinished3(QNetworkReply *reply);
    void replyFinished4(QNetworkReply *reply);
    void writeUrl();

private:
    QNetworkAccessManager *network_manager;
    QNetworkAccessManager *network_manager2;
    QNetworkAccessManager *network_manager3;
    QNetworkAccessManager *network_manager4;
    QNetworkRequest *network_request;
    QNetworkRequest *network_request2;
    QNetworkRequest *network_request3;
    QNetworkRequest *network_request4;
    QList<long> specialId;
    QList<QString> m_specialName;
    QList<QString> m_nickName;
    QList<double> m_playCount;
    QList<QString> m_songName;
    QList<QString> m_singerName;
    QList<QString> m_albumName;
    QList<QString> hash;
    QList<long> album_id;
    QString m_image;
    QString m_lyrics;
    QString m_url;
    bool isDownloadSong=false;
    QString m_savePath;
};

#endif // KUGOUPLAYLIST_H
