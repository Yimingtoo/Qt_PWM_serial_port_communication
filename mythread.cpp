#include "mythread.h"
#include "usart.h"

Mythread::Mythread(QObject *parent) : QThread(parent)
{
//    leftbuff="";
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
//        int count=0;
        float data;
        QByteArray receive;
//        QPolygonF temppoints;
        QPointF temppoint;
//        float tempmax,tempmin;
//        int maxindex=-1,minindex=-1;
//        buff=leftbuff+buff;
        //qDebug()<<buff;
        while(buff.length()>=6)
        {
            switch(buff.left(1).toHex().toInt(&ok,16))
            {
            case REC_FLOAT:
                receive=buff.left(5).right(4);
                buffer=receive.data();
                data=*((float*)buffer);

                //qDebug()<<data;//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

                //qDebug()<<count;
//                if(count==0)
//                {
//                    tempmax=data;
//                    tempmin=data;
//                    maxindex=0;
//                    minindex=0;
//                }
//                if(data>tempmax)
//                {
//                    tempmax=data;
//                    maxindex=count;
//                }
//                if(data<tempmin)
//                {
//                    tempmin=data;
//                    minindex=count;
//                }

                //获取时间
//                temppoints<<QPointF(currentTime,data);

                temppoint=QPointF(currentTime,data);
                buff=buff.remove(0,6);
//                count++;
                break;
            default:
                buff=buff.remove(0,1);
                break;
            }
        }
//        if(buff.left(1).toHex().toInt(&ok,16)==REC_FLOAT)
//            leftbuff=buff;
//        qDebug()<<minindex<<maxindex;

        emit float_isDone(temppoint);
        Tflag=FLAG_CLEAR;   //清除标志位
    }

    //qDebug()<<"over";
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

//void Mythread::reset_leftbuff()
//{
//    leftbuff="";
//}
