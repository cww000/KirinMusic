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
#ifndef SONG_H
#define SONG_H

#include <QObject>
#include <QVariantMap>
class Song : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap Tags READ Tags WRITE setTags NOTIFY TagsChanged)
    Q_PROPERTY(bool flag READ flag WRITE setflag NOTIFY flagChanged)
public:
    explicit Song(QObject *parent = nullptr);
    Q_INVOKABLE void getTags(QString url, QString dirPath);
    Q_INVOKABLE void saveTags(QString url,QVariantMap map);

    void mp3Open(const char *ch);
    void flacOpen(const char *ch);
    void oggOpen(const char *ch);
    void wavOpen(const char *ch);
    void mp3Save(const char *ch,QVariantMap map);
    void flacSave(const char *ch,QVariantMap map);
    void oggSave(const char *ch,QVariantMap map);
    void wavSave(const char *ch,QVariantMap map);
    void clearTags();
    QVariantMap Tags() const
    {
        return m_Tags;
    }

    bool flag() const;
    void setflag(bool newFlag);

public slots:
    void setTags(QVariantMap Tags)
    {
        if (m_Tags == Tags)
            return;

        m_Tags = Tags;
        emit TagsChanged(m_Tags);
    }

signals:
    void TagsChanged(QVariantMap Tags);

    void flagChanged();

private:
    QVariantMap m_Tags;
    bool m_flag;
    QString m_pic;
};
#endif // SONG_H
