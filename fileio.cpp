#include "fileio.h"
#include <QTextStream>
#include <QFile>
#include <QVector>
#include <fstream>
#include <QDebug>
#include <QDir>
FileIo::FileIo(QObject *parent) : QObject(parent)
{

}

QString FileIo::read()
{
    QString content;
    QFile file(m_source);
    QTextStream in(&file);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        content=in.readAll();
        file.close();
    }
    return content;
}

void FileIo::write(const QString &data)
{
   QFile file(m_source);
   bool ok=file.open(QIODevice::WriteOnly|QIODevice::Text);
   if(ok) {
       QTextStream out(&file);
       out<<data;
       file.close();
   }
}

//将地址保存到播放列表
void FileIo::saveUrls(QList<QUrl> urls)
{
    QDir dir("/tmp/KirinMusic");
    if(!dir.exists()){
        dir.mkdir("/tmp/KirinMusic");
    }
    QFile file("/tmp/KirinMusic/播放列表.txt");
    bool ok=file.open(QIODevice::WriteOnly|QIODevice::Append);
    if(ok) {
        int length=urls.count();
        QTextStream out(&file);
        for(int i=0;i<length;i++) {
            QString path=urls[i].toString().remove(0,7);
            out<<path<<"\n";
        }
        file.close();
    }
}


//读取播放列表中的某条地址
void FileIo::readUrls(int serialNumber, QString fileUrl)
{
    QFile file(fileUrl);
    QTextStream in(&file);
    int index=0;
    bool ok=file.open(QIODevice::ReadOnly|QIODevice::Text);
    if(ok) {
        while(!in.atEnd()) {
            QString path=in.readLine();
            if(index==serialNumber) {
                m_source=path;
                break;
            }
            index++;
        }
        file.close();
    }else{

    }
}

void FileIo::deleteUrls(int serialNumber, QString fileUrl)
{
    QFile file(fileUrl);
//    qDebug()<<serialNumber;
    QTextStream in(&file);
    QString str, path;
    int index=0;
    bool ok=file.open(QIODevice::ReadOnly|QIODevice::Text);
    if(ok) {
        while(!in.atEnd()) {
            path=in.readLine();
            if(index!=serialNumber) {
                str = str + path + "\n";
            }
            index++;
        }
        file.close();
    }
    file.open(QIODevice::WriteOnly|QIODevice::Truncate);     //清空file
    in<<str;
    file.close();
}

void FileIo::deleteAllUrls(QString fileUrl)
{
    QFile file(fileUrl);
    file.open(QIODevice::WriteOnly|QIODevice::Truncate);     //清空file
    file.close();
}
void FileIo::saveKeys(QList<QString> keys)
{
    QFile file("/tmp/KirinMusic/快捷键.txt");
    int length=keys.length();
    QString path;
    bool ok=file.open(QIODevice::ReadWrite|QIODevice::Truncate);
    if(ok) {
        QTextStream in(&file);
        for(int i=0;i<length;i++){
            in<<keys[i]<<"\n";
        }

        file.close();
    }
}

QString FileIo::readKey(int serialNumber)
{
    QFile file("/tmp/KirinMusic/快捷键.txt");
    QString str;
    QTextStream in(&file);
    int index = 0;
    bool ok=file.open(QIODevice::ReadWrite|QIODevice::Text);
    if(ok) {
        while(!in.atEnd()) {
            str=in.readLine();
            if(index==serialNumber) {
                break;
            }
            index++;
        }
        file.close();
    }
    return str;
}

//得到播放列表中的地址
void FileIo::getPlaylist()
{
    QFile file("/tmp/KirinMusic/播放列表.txt");
    QTextStream in(&file);
    int index = 0;
    bool ok=file.open(QIODevice::ReadOnly);
    if(ok) {
        while(!in.atEnd()) {
            QString path=in.readLine();
            QFile fileUrl(path);
            bool ok=fileUrl.open(QIODevice::ReadOnly);
            if(ok){
                m_urls.push_back(path);
            }else{
                deleteUrls(index, "/tmp/KirinMusic/播放列表.txt");
            }
            index++;
        }
        file.close();
    }
}

//判断某首歌地址在播放列表中是否已经存在
bool FileIo::isExist(QString url)
{
    QFile file("/tmp/KirinMusic/播放列表.txt");
    QTextStream in(&file);
    bool ok=file.open(QIODevice::ReadOnly);
    if(ok) {
        while(!in.atEnd()) {
            QString path=in.readLine();
            QFile fileUrl(path);
            bool ok=fileUrl.open(QIODevice::ReadOnly);
            if(ok){
                if(url==path) {
                    return false;
                }
            }
        }
        file.close();
    }
    return true;
}

void FileIo::saveRecentlyUrls(QList<QUrl> urls)
{
    QFile file("/tmp/KirinMusic/最近播放.txt");
    int length=urls.length();
    qDebug()<<length;

    for(int i=0;i<length;i++) {
        bool ok=file.open(QIODevice::ReadWrite);
        if(ok) {
            QTextStream in(&file);
            QString path=urls[i].toString().remove(0,7);
            QString str = path + "\n";
            while(!in.atEnd()) {
                QString recentlypath=in.readLine();
                if(path!=recentlypath) {
                    str = str + recentlypath + "\n";
                }
            }
            file.close();
            file.open(QIODevice::WriteOnly|QIODevice::Truncate);
            in<<str;
            file.close();
        }
    }
}

void FileIo::getRecentlyPlaylist()
{
    QFile file("/tmp/KirinMusic/最近播放.txt");
//    qDebug()<<serialNumber;
    m_recentlyUrls.clear();
    QTextStream in(&file);
    int index = 0;
    bool ok=file.open(QIODevice::ReadOnly);
    if(ok) {
        while(!in.atEnd()) {
            QString path=in.readLine();
            QFile fileUrl(path);
            bool ok=fileUrl.open(QIODevice::ReadOnly);
            if(ok){
                m_recentlyUrls.push_back(path);
            }else{
                deleteUrls(index, "/tmp/KirinMusic/最近播放.txt");
            }
            index++;
        }
        file.close();
    }
}

//得到目录下的文件
QList<QString> FileIo::getFiles(QString path)
{
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "*.mp3" << "*.ogg"<<"*.flac";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    QList<QString> url;
    for(int i=0;i<files.length();i++) {
        if(isExist(path+"/"+files[i])) {
            url<<files[i];
        }
    }
    return url;
}


QUrl FileIo::strToUrl(QString str)
{
    QUrl url(str);
    return url;
}

