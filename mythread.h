#ifndef MYTHREAD_H
#define MYTHREAD_H

#define FLAG_CLEAR 0
#define REC_FLAG 1
#define DRAW_FLOAT_FLAG 2

#include <QThread>
#include <QDebug>
#include <QPolygonF>

class Mythread : public QThread
{
    Q_OBJECT
public:
    explicit Mythread(QObject *parent = nullptr);

protected:

    //线程处理函数
    void run();

signals:

    void hex_isDone(QByteArray send);

    void float_isDone(QPointF point);

private:

    QByteArray buff;

    int Tflag;

    float currentTime;

public:

    void setbuff(QByteArray buffer);

    void setflag(int flag);

    void settime(float time);

};

#endif // MYTHREAD_H
