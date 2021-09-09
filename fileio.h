#ifndef FILECONTENT_H
#define FILECONTENT_H

#include <QObject>
#include <QUrl>
#include <QList>
class FileIo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QList<QUrl> urls READ urls WRITE setUrls NOTIFY urlsChanged)
    Q_PROPERTY(QList<QUrl> recentlyUrls READ recentlyUrls WRITE setRecentlyUrls NOTIFY recentlyUrlsChanged)
public:
    explicit FileIo(QObject *parent = nullptr);
    Q_INVOKABLE QString read();
    Q_INVOKABLE void write(const QString &data);
    Q_INVOKABLE void saveUrls(QList<QUrl> urls);
    Q_INVOKABLE void readUrls(int serialNumber, QString fileUrl);
    Q_INVOKABLE void deleteUrls(int serialNumber, QString fileUrl);
    Q_INVOKABLE void deleteAllUrls(QString fileUrl);
    Q_INVOKABLE void saveKeys(QList<QString> keys);
    Q_INVOKABLE QString readKey(int serialNumber);
    Q_INVOKABLE void getPlaylist();
    Q_INVOKABLE void saveRecentlyUrls(QList<QUrl> urls);
    Q_INVOKABLE void getRecentlyPlaylist();
    Q_INVOKABLE QList<QString> getFiles(QString path);  //得到目录下的所有文件
    Q_INVOKABLE bool isExist(QString url);
    Q_INVOKABLE QUrl strToUrl(QString str);
    void setSource(const QString &source){m_source=source;}
    QString source() {return m_source;}
    QList<QUrl> urls() const
    {
        return m_urls;
    }

    QList<QUrl> recentlyUrls() const
    {
        return m_recentlyUrls;
    }

public slots:
    void setUrls(QList<QUrl> urls)
    {
        if (m_urls == urls)
            return;

        m_urls = urls;
        emit urlsChanged(m_urls);
    }

    void setRecentlyUrls(QList<QUrl> recentlyUrls)
    {
        if (m_recentlyUrls == recentlyUrls)
            return;

        m_recentlyUrls = recentlyUrls;
        emit recentlyUrlsChanged(m_recentlyUrls);
    }

signals:
    void sourceChanged(const QString &source);
    void urlsChanged(QList<QUrl> urls);

    void recentlyUrlsChanged(QList<QUrl> recentlyUrls);

private:
    QString m_source;
    QList<QUrl> m_urls;
    QList<QUrl> m_recentlyUrls;
};

#endif // FILECONTENT_H
