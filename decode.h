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
#ifndef DECODE_H
#define DECODE_H

#include <QObject>

class Decode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<float> vertices READ vertices WRITE setvertices NOTIFY verticesChanged)
public:
    explicit Decode(QObject *parent = nullptr);
    Q_INVOKABLE bool decode(QString filePath, QString dirPath);
    void getVertic(QString filePath);
    QList<float> vertices() const
    {
        return m_vertices;
    }

public slots:
    void setvertices(QList<float> vertices)
    {
        if (m_vertices == vertices)
            return;

        m_vertices = vertices;
        emit verticesChanged(m_vertices);
    }

signals:

    void verticesChanged(QList<float> vertices);

private:
    QList<float> m_vertices;
};

#endif // DECODE_H
