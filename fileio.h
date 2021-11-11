/*
* author: 程炆炆，聂海艳，李纯林
* email：1460964870@qq.com 1933544851@qq.com 2742731130@qq.com
* time:2021.10

* Copyright (C) <2021>  <Wenwen Cheng,Haiyan Nie,Chunlin Li>

* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef FILECONTENT_H
#define FILECONTENT_H

#include <QObject>
#include <QUrl>
#include <QList>
class FileIo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString dirPath READ dirPath WRITE setDirPath NOTIFY dirPathChanged)
    Q_PROPERTY(QList<QUrl> urls READ urls WRITE setUrls NOTIFY urlsChanged)
    Q_PROPERTY(QList<QUrl> recentlyUrls READ recentlyUrls WRITE setRecentlyUrls NOTIFY recentlyUrlsChanged)
public:
    explicit FileIo(QObject *parent = nullptr);
    Q_INVOKABLE QString read();
    Q_INVOKABLE void write(const QString &data);
    Q_INVOKABLE void saveUrls(QList<QUrl> urls);
    Q_INVOKABLE void saveBackgroundUrl(QUrl url);
    Q_INVOKABLE void readUrls(int serialNumber, QString fileUrl);    //read music url
    Q_INVOKABLE void readBackgroundUrl(QString fileUrl);
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

    const QString &dirPath() const;
    void setDirPath(const QString &newDirPath);

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

    void dirPathChanged();

private:
    QString m_source;
    QList<QUrl> m_urls;
    QList<QUrl> m_recentlyUrls;
    QString m_dirPath;
};

#endif // FILECONTENT_H
