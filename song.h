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
    Q_INVOKABLE void getTags(QString url);
    Q_INVOKABLE void saveTags(QString url,QVariantMap map);

    void mp3Open(const char *ch);
    void flacOpen(const char *ch);
    void oggOpen(const char *ch);
    void mp3Save(const char *ch,QVariantMap map);
    void flacSave(const char *ch,QVariantMap map);
    void oggSave(const char *ch,QVariantMap map);
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
};
#endif // SONG_H
