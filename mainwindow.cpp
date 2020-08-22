#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QPainter>
#include <QLineF>
#include <QPointF>
#include <QDebug>
#include <QMouseEvent>
#include <QTimer>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->draw_init();
    this->usart_init();

    ui->bandRateBox->setEditable(true);
    connect(&serial,&QSerialPort::readyRead ,this,&MainWindow:: receive);
    test1();

    //------------------------------------------//

    connect(ui->usartSettingsAction,&QAction::triggered,this,[=]()
    {
       ui->dockWidget->show();
    });
    //points<<QPointF(1,10000000000);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//以下部分主要使用mfc，具体请参考msdn
//热插拔事件
bool MainWindow:: nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    MSG* msg = reinterpret_cast<MSG*>(message);
    int msgType = msg->message;
    if(msgType == WM_DEVICECHANGE)
    {
        qDebug() << "Recv Event " ;
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        switch(msg->wParam)
        {
            case DBT_DEVICETYPESPECIFIC:
            {
                qDebug() << "DBT_DEVICETYPESPECIFIC " ;
                break;
            }
            case DBT_DEVICEARRIVAL: //当有usb插入时
            if (lpdb -> dbch_devicetype == DBT_DEVTYP_PORT) //筛选是否为串口 msdn
            {
                ui->comBox->clear();
                foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
                {
                    QString item;
                    item=info.portName()+" "+info.description();
                    ui->comBox->addItem(item);
                }
            }
            qDebug() << "DBT_DEVICEARRIVAL" ;
            break;
        case DBT_DEVICEREMOVECOMPLETE:  //当有usb拔出时
            if (lpdb -> dbch_devicetype == DBT_DEVTYP_PORT) //筛选是否为串口
            {
                ui->comBox->clear();
                foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
                {
                    QString item;
                    item=info.portName()+" "+info.description();
                    ui->comBox->addItem(item);
                }
            }
            qDebug() << "DBT_DEVICEREMOVECOMPLETE" ;
            break;
        }
    }
    return false;
}

void MainWindow::draw_init()
{
    drag_flag=false;
    timerIsRun=false;
    accuracyx=2;

    ymin=0;
    ymax=4;

    ui->qwtPlot->setTitle("波形输出");

    rect = ui->qwtPlot->geometry();
    qwtw=rect.width()-60;
    qwth=rect.height()-80;

    timecount=0;

    peak.setX(0);
    peak.setY(0);
    valley.setX(0);
    valley.setY(0);
    index_peak=0;
    index_valley=0;

    fix_ymax=ymax;
    fix_ymin=ymin;
    ui->ymaxSpinBox->setValue(fix_ymax);
    ui->yminSpinBox->setValue(fix_ymin);
    firstpoint_flag=true;

    ui->yminSpinBox->setMinimum(-100000);
    ui->ymaxSpinBox->setMinimum(-100000);

}

void MainWindow::test1()
{
    QwtPlotCanvas *canvas=new QwtPlotCanvas();//只能这种方法新建，否则程序会异常结束
    canvas->setPalette(Qt::white);
    ui->qwtPlot->setCanvas(canvas);//设置画布
    ui->qwtPlot->plotLayout()->setAlignCanvasToScales(true);//对齐画布到轴比例

    //设置xy轴标题
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft,"y轴");
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom,"x轴");

    //设置网格线
    QwtPlotGrid *grid=new QwtPlotGrid();
    grid->enableX( true );
    grid->enableY( true );
    grid->setMajorPen(Qt::black,0,Qt::DotLine);//设置网格线为黑细点线
    grid->attach(ui->qwtPlot);//设置网格线

    //绘图
    QwtPlotCurve *curve=new QwtPlotCurve("curve");//新建曲线句柄,构造函数输入的内容为图例的名字
    curve->setPen(Qt::blue,2);//设置曲线为蓝色，线粗为2
    curve->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化
    QwtSymbol *symbol=new QwtSymbol(QwtSymbol::Ellipse,QBrush(Qt::yellow),QPen(Qt::red,2),QSize(6,6));
    curve->setSymbol(symbol);

    //绘直线的第二种方法
    QPointF point(0,0);

    points<<point;
    curve2=new QwtPlotCurve("curve2");
    curve2->setPen(Qt::blue,2);//设置曲线为蓝色，线粗为2

    curve2->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化

    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,ymin,ymax,1);
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,xmin,xmax,2);
    ui->qwtPlot->setAutoReplot(true);//设置自动重画，相当于更新
}

void MainWindow::paintEvent(QPaintEvent *)
{
    static int tempflag=4;
    if(tempflag)
    {
        double scalex=qwtw/36;
        xmax=xmin+scalex;
        ui->qwtPlot->setAxisScale(QwtPlot::xBottom,xmin,xmax,accuracyx);
        tempflag--;
    }

    //每次重绘时都重新获取长宽
    qwtw=ui->qwtPlot->geometry().width()-60;
    qwth=ui->qwtPlot->geometry().height()-80;


    //坐标自动移动
    //qDebug()<<drag_flag;
    if(drag_flag==false)//如果移动则停止自动移动
    {
        if(points.at(points.size()-1).x()-xmax>5)
        {
            xmin=xmin+points.at(points.size()-1).x()-xmax+1;
            xmax=xmax+points.at(points.size()-1).x()-xmax+1;

            ui->qwtPlot->setAxisScale(QwtPlot::xBottom,xmin,xmax,accuracyx);
        }
        if(points.at(points.size()-1).x()>xmax-1)
        {
            xmax+=0.04;
            xmin+=0.04;
            ui->qwtPlot->setAxisScale(QwtPlot::xBottom,xmin,xmax,accuracyx);
        }
        //qDebug()<<(rect.width())<<(ui->qwtPlot->geometry().width());

        if(((rect.width()/10)!=(ui->qwtPlot->geometry().width())/10))
        {
            double scalex=qwtw/36;
            xmax=xmin+scalex;
            ui->qwtPlot->setAxisScale(QwtPlot::xBottom,xmin,xmax,accuracyx);
            rect = ui->qwtPlot->geometry();
        }
    }

    tempIntervalX =ui->qwtPlot->axisScaleDiv(QwtPlot::xBottom).interval();
    xmin=tempIntervalX.minValue();
    xmax=tempIntervalX.maxValue();
    spanx=xmax-xmin;

    accuracyx=60*spanx/qwtw;
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,xmin,xmax,accuracyx);

    if(fix_flag==false)
    {
        if(points.at(points.size()-1).y()>ymax-1)
        {
            ymax=points.at(points.size()-1).y()+1;
        }
        if(points.at(points.size()-1).y()<ymin+1)
        {
            ymin=points.at(points.size()-1).y()-1;
        }

        spany=ymax-ymin;
        accuracyy=int(60*spany/qwth);
        ui->qwtPlot->setAxisScale(QwtPlot::yLeft,ymin,ymax,accuracyy);
    }
    else
    {
        fix_ymax=ui->ymaxSpinBox->value();
        fix_ymin=ui->yminSpinBox->value();
        spany=fix_ymax-fix_ymin;


        if(spany<=0.5)
        {
            return;
        }

        accuracyy=int(60*spany/qwth);
        ui->qwtPlot->setAxisScale(QwtPlot::yLeft,fix_ymin,fix_ymax,accuracyy);
    }

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPointF point = event->pos();//窗口内位置
    qDebug()<<"("<<point.x()<<","<<point.y()<<")";
    if(ui->qwtPlot->geometry().contains(this->mapFromGlobal(QCursor::pos())))
    {
        globalpoint=point;
    }
}

void MainWindow:: mouseMoveEvent(QMouseEvent *event)
{
    if(ui->qwtPlot->geometry().contains(this->mapFromGlobal(QCursor::pos())))
    {
        drag_flag=true;

        QPointF point = event->pos();//窗口内位置

        qreal dx=0;
        if(ui->qwtPlot->geometry().contains(this->mapFromGlobal(QCursor::pos())))
        {
            dx=point.x()-globalpoint.x();

            double move=dx/(qwtw/spanx);
            xmin=xmin-move;
            xmax=xmax-move;
            ui->qwtPlot->setAxisScale(QwtPlot::xBottom,xmin,xmax,accuracyx);
        }
        globalpoint=point;
    }
}

void MainWindow:: mouseReleaseEvent(QMouseEvent *event)
{


}

void MainWindow:: wheelEvent(QWheelEvent *event)
{
    if(ui->qwtPlot->geometry().contains(this->mapFromGlobal(QCursor::pos())))
    {
        //获取鼠标位置
        QPointF point = event->pos();//窗口内位置
        double currentx=xmin+(point.x()-rect.x()-60)/(qwtw/spanx);//转换鼠标所在刻度


        if(event->delta()>0)
        {
            //qDebug()<<"放大"<<currentx;
            xmin=currentx-0.95*(currentx-xmin);
            xmax=currentx+0.95*(xmax-currentx);
            spanx=xmax-xmin;
            ui->qwtPlot->setAxisScale(QwtPlot::xBottom,xmin,xmax,accuracyx);

        }
        else
        {
            //qDebug()<<"缩小";
            xmin=currentx-1.05*(currentx-xmin);
            xmax=currentx+1.05*(xmax-currentx);
            spanx=xmax-xmin;
            ui->qwtPlot->setAxisScale(QwtPlot::xBottom,xmin,xmax,accuracyx);
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    drag_flag=false;
    spanx=qwtw/36;
    xmin=points.at(points.size()-1).x()-2;
    xmax=xmin+spanx;

    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,xmin,xmax,accuracyx);

}


void MainWindow::on_startButton_clicked()
{
    if(this->openUSART())
    {
        if(timerIsRun==false)
        {
            timerID=this->startTimer(10);
            ui->startButton->setText("结束");
            timerIsRun=true;
            ui->resetButton->setEnabled(false);
        }
        else if(timerIsRun==true)
        {
            this->killTimer(timerID);
            ui->startButton->setText("继续");
            timerIsRun=false;
            ui->resetButton->setEnabled(true);
        }
    }
    else
    {
        return;
    }


}

void MainWindow:: timerEvent(QTimerEvent *e)
{
    if(e->timerId()==timerID)
    {
        timecount++;
        ui->label->setText(QString::number(timecount/100.0,'f',2));
        //qDebug()<<"timer"<<timecount;
    }
    else if(e->timerId()==timerID2)
    {
        ui->comBox->clear();
        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            QString item;
            item=info.portName()+" "+info.description();
            ui->comBox->addItem(item);
        }
        //qDebug()<<"timer";
    }
//    points<<QPointF(timecount/100.0,-timecount/100.0);
//    curve2->setSamples(points);
//    curve2->attach(ui->qwtPlot);
//    curve2->setLegendAttribute(curve2->LegendShowLine);
}

void MainWindow::on_resetButton_clicked()
{
    //重置拖拽标志
    drag_flag=false;

    //清除数据点
    points.clear();
    points<<QPointF(0,0);

    //重置峰值点和谷值点
    peak.setX(0);
    peak.setY(0);
    valley.setX(0);
    valley.setY(0);
    index_peak=0;
    index_valley=0;

    //重置x轴
    spanx=qwtw/36;
    xmin=points.at(points.size()-1).x()-2;
    xmax=xmin+spanx;
    accuracyx=60*spanx/qwtw;
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,xmin,xmax,accuracyx);

    //重置y轴
    ymin=0;
    ymax=4;
    accuracyy=60*spany/qwth;
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,ymin,ymax,accuracyy);

    //重置计数器
    timecount=0;

    //重置画图
    ui->label->setNum(0);
    curve2->setSamples(points);
    curve2->attach(ui->qwtPlot);
    curve2->setLegendAttribute(curve2->LegendShowLine);

    //重置清除第一点标志位
    firstpoint_flag=true;

    //设置按键文本
    ui->startButton->setText("开始");

    //重置峰谷点
    ui->valleylabel->setText("( 0 , 0 )");
    ui->peaklabel->setText("( 0 , 0 )");
}

void MainWindow::on_fixButton_clicked()
{
    if(fix_flag==false)
    {
        fix_flag=true;
        ui->fixButton->setText("解固");
    }
    else
    {
        fix_flag=false;
        ui->fixButton->setText("固定");
    }
}










