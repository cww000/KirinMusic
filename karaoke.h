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
    Q_PROPERTY(QString hash READ hash WRITE setHash NOTIFY hashChanged)

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

    const QString &hash() const;
    void setHash(const QString &newHash);

signals:
    void urlChanged();

    void lyricsChanged();

    void hashChanged();

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
    QString m_hash;
    QString m_url;
    QString m_lyrics;

};

#endif // KARAOKE_H
