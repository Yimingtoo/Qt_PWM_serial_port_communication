#include "mythread.h"
#include "usart.h"

Mythread::Mythread(QObject *parent) : QThread(parent)
{

}

void Mythread::run()
{
    if(REC_FLAG==Tflag)
    {
        QByteArray temp="";
        while(buff!="\0")
        {
            temp=temp+"\t"+buff.left(2);
            buff.remove(0,2);
        }
        temp=temp.remove(0,1)+"\t";
        emit hex_isDone(temp);

        Tflag=FLAG_CLEAR;   //清除标志位
    }
    else if(DRAW_FLOAT_FLAG==Tflag)
    {
        bool ok;
        char* buffer;
        float data;
        QByteArray receive;
        QPointF temppoint;

        while(buff.length()>=6)
        {
            switch(buff.left(1).toHex().toInt(&ok,16))
            {
            case REC_FLOAT:
                receive=buff.left(5).right(4);
                buffer=receive.data();
                data=*((float*)buffer);
                temppoint=QPointF(currentTime,data);
                buff=buff.remove(0,6);
                break;

            default:
                buff=buff.remove(0,1);
                break;
            }
        }
        emit float_isDone(temppoint);
        Tflag=FLAG_CLEAR;   //清除标志位
    }
}

void Mythread::setbuff(QByteArray buffer)
{
    buff=buffer;
}

void Mythread::setflag(int flag)
{
    switch (flag)
    {
    case REC_FLAG:
        Tflag=REC_FLAG;
        break;
    case DRAW_FLOAT_FLAG:
        Tflag=DRAW_FLOAT_FLAG;
        break;
    default:
        break;
    }
}

void Mythread::settime(float time)
{
    currentTime=time;
}


