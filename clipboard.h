/*
* author: 程炆炆，聂海艳，李纯林
* email：1460964870@qq.com 1933544851@qq.com 2742731130@qq.com
* time:2021.10
* Copyright (C) <2021>  <Wenwen Cheng,Haiyan Nie,Chunlin Li>
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QGuiApplication>
#include <QObject>
#include <QClipboard>

class Clipboard : public QObject
{
    Q_OBJECT
public:
    explicit Clipboard(QObject *parent = nullptr);
    Q_INVOKABLE void setText(QString text);
private:
    QClipboard *clipboard;
};

#endif // CLIPBOARD_H

