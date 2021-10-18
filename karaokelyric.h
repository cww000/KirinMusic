#ifndef KARAOKELYRIC_H
#define KARAOKELYRIC_H

#include <QObject>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QList>
class LyricLine;

typedef int  CONVERT_CODE;
typedef QList<int> List;
class KaraokeLyric : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString hash READ hash WRITE setHash NOTIFY hashChanged)
    Q_PROPERTY(QList<QString> plainLyric READ plainLyric WRITE setPlainLyric NOTIFY plainLyricChanged)
    Q_PROPERTY(QList<int> startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(QList<int> lineDuration READ lineDuration WRITE setLineDuration NOTIFY lineDurationChanged)
    Q_PROPERTY(int timeDif READ timeDif WRITE setTimeDif NOTIFY timeDifChanged)
public:
    explicit KaraokeLyric(QObject *parent = nullptr);
    ~KaraokeLyric();
    Q_INVOKABLE void lyricSearch();
    Q_INVOKABLE int findTimeInterval(QString nowTime);
    void parseJson_getID(QString result);
    void parseJson_getLyrics(QString result);
    void getLyric();
    CONVERT_CODE krcDecode(QByteArray krcData);
    int translate(QString time);

    const QString &hash() const;
    void setHash(const QString &newHash);

    const QList<QString> &plainLyric() const;
    void setPlainLyric(const QList<QString> &newPlainLyric);

    const QList<int> &startTime() const;
    void setStartTime(const QList<int> &newStartTime);

    const QList<int> &lineDuration() const;
    void setLineDuration(const QList<int> &newLineDuration);

    int timeDif() const;
    void setTimeDif(int newTimeDif);

signals:
    void hashChanged();

    void plainLyricChanged();

    void startTimeChanged();

    void lineDurationChanged();

    void timeDifChanged();

protected slots:
    void replyFinished(QNetworkReply *reply);
    void replyFinished2(QNetworkReply*reply);


private:
    QNetworkAccessManager *network_manager;
    QNetworkAccessManager *network_manager2;
    QNetworkRequest *network_request;
    QNetworkRequest *network_request2;


    QString id;
    QString accesskey;
    QList<QString> lyric;
    double m_duration;
    double m_score;
    QString m_hash;

    LyricLine *m_lyricLine;
    QList<QString> m_plainLyric;          //纯歌词
    QList<int> m_startTime;        //每行歌词的开始时间
    QList<int> m_lineDuration;       //每行歌词的结束时间
    QList<List> m_wordDuration;     //每行歌词中每个字的持续时间
    int m_timeDif;


};

#endif // KARAOKELYRIC_H
