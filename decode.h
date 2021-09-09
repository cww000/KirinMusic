#ifndef DECODE_H
#define DECODE_H

#include <QObject>

class Decode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<float> vertices READ vertices WRITE setvertices NOTIFY verticesChanged)
public:
    explicit Decode(QObject *parent = nullptr);
    Q_INVOKABLE bool decode(QString filePath);
    void getVertic();
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
