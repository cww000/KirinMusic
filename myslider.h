#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QSlider>
#include <QObject>

class MySlider : public QSlider
{
public:
    MySlider();
signals:
    void sliderReleased();
protected:
    void mouseReleaseEvent(QMouseEvent *ev) override;
};

#endif // MYSLIDER_H
