#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWindow_init();

    //-----------------------------------------信号与槽-----------------------------------------//

    connect(&serial,&QSerialPort::readyRead ,this,&MainWindow:: receive);

    connect(ui->usartSettingsAction,&QAction::triggered,this,[=]()
    {
       ui->dockWidget->show();
    });

    connect(thread,&Mythread::hex_isDone,this,&MainWindow::dealThread);

    connect(this,&MainWindow::destroyed,this,&MainWindow::stopThread);

    connect(ui->tebWidget,&QTabWidget::currentChanged,this,&MainWindow::tabchange);

    connect(thread,&Mythread::float_isDone,this,&MainWindow::dealfloatThread);

    connect(ui->pinsettingAction,&QAction::triggered,this,[=]()
    {
        setting->show();
        setting->lineEdit_clear();
    });

    connect(setting,&Setting::pinsetting,this,&MainWindow::pinsettingrec);

    connect(ui->resetsetAction,&QAction::triggered,this,&MainWindow::write_resetvalue);

    connect(setting,&Setting::wheel_open,this,&MainWindow::wheel_open_rec);

    connect(setting,&Setting::addbandrate,this,&MainWindow::bandrate_rec);

    connect(ui->bandRateBox, QOverload<int>::of(&QComboBox::highlighted),this,&MainWindow::bandrate_ishighlighted);

    connect(ui->bandRateBox, QOverload<int>::of(&QComboBox::currentIndexChanged),this,&MainWindow::bandrateindex_ischanged);

    connect(setting,&Setting::PID_open,this,&MainWindow::PID_open_rec);

    connect(ui->saveas_action,&QAction::triggered,this,&MainWindow::saveas_action_res);

    connect(ui->writereset_action,&QAction::triggered,this,&MainWindow::writereset_action_res);

    connect(ui->debug_save_action,&QAction::triggered,this,&MainWindow::debug_save_action_rec);

    connect(ui->debug_save_as_action,&QAction::triggered,this,&MainWindow::debug_save_as_action_rec);

    connect(ui->open_action,&QAction::triggered,this,&MainWindow::open_action_rec);

    connect(ui->open_from_action,&QAction::triggered,this,&MainWindow::open_from_action_rec);

    //-----------------------------------------结尾处理-----------------------------------------//

    setting->getSettingStatus();
    this->tabchange();

    qDebug()<<download_flag<<part_download_flag;

    //ui->repeat_download_Button->setVisible(false);

}

MainWindow::~MainWindow()
{
    bandrate_destruct();
    delete ui;
}

//------------------------------------------------------------------------------------------------>自定义函数

void MainWindow::MainWindow_init()
{
    //初始化
    this->draw_init();
    this->usart_init();
    this->bandrate_init();

    thread=new Mythread(this);

    //为设置模态对话框分配内存
    setting=new Setting;
    setting->setWindowModality(Qt::ApplicationModal);

    this->steering_gear_control_init();

    timerIsRun=false;
    timer2IsRun=false;
    timer3IsRun=false;

    this->setWindowTitle("调试助手");

    this->setWindowIcon(QIcon("./bug.ico"));

}

void MainWindow::wheel_open_rec(bool isok)
{
    if(isok)
    {
        wheel_steer_flag=true;
    }
    else
    {
        wheel_steer_flag=false;
    }
}

void MainWindow::group_wheel(int group,QGroupBox *box,QWheelEvent *event)
{
    int left=box->geometry().x();
    int right=left+box->geometry().width();
    int up=box->geometry().y();
    int down=up+box->geometry().height();
    int posx=ui->qwtPlot->mapFrom(this,this->mapFromGlobal(QCursor::pos())).x();
    int posy=ui->qwtPlot->mapFrom(this,this->mapFromGlobal(QCursor::pos())).y();

    if(left<posx && posx<right && up<posy && posy<down)
    {
        if(event->delta()>0)
        {
            group_find_value=TIME_GROUP(group,FORWARD);
        }
        else
        {
            group_find_value=TIME_GROUP(group,BACK);
        }
        group_find();
    }
}

//------------------------------------------------------------------------------------------------>槽函数

void MainWindow::PID_open_rec(bool isok)
{
    if(isok)
    {
        isPIDopen_flag=true;
        PID_setvisible(true);
        this->tabchange();

    }
    else
    {
        isPIDopen_flag=false;
        PID_setvisible(false);

        goalpoints.clear();
        goalpoints<<QPointF(0,0)<<QPointF(0,0);
        curve1->setSamples(goalpoints);
        curve1->attach(ui->qwtPlot);
        curve1->setLegendAttribute(curve2->LegendShowLine);
    }
}

void MainWindow::tabchange()
{

    if(ui->open_Button->text() == QString("关闭串口"))
        this->reverseUSART();  //关闭串口

    if(ui->tebWidget->currentIndex()==2)
    {
        ui->codetableWidget->setVisible(true);
        ui->part_down_Button->setVisible(true);
        ui->repeat_download_Button->setVisible(true);
    }
    else
    {
        ui->codetableWidget->setVisible(false);
        ui->part_down_Button->setVisible(false);
        ui->repeat_download_Button->setVisible(false);
    }

    if(ui->tebWidget->currentIndex()==0)
    {
        ui->open_Button->setVisible(false);
        if(isPIDopen_flag)
        {
            PID_setvisible(true);
        }
    }
    else
    {
        ui->open_Button->setVisible(true);
        PID_setvisible(false);

        if(timerIsRun==true)
        {
            this->killTimer(timerID);
            ui->startButton->setText("继续");
            timerIsRun=false;
            ui->resetButton->setEnabled(true);
        }
    }
}

void MainWindow::dealThread(QByteArray buffer)
{
    qDebug()<<buffer;

    if(ui->tebWidget->currentIndex()==1 || ui->tebWidget->currentIndex()==2)
    {
        ui->ReceiveEdit->insertPlainText((QString) buffer);
    }
}

void MainWindow::dealfloatThread(QPointF point)
{
    if(ui->tebWidget->currentIndex()==0)
    {
        if(firstpoint_flag)
        {
            firstpoint_flag=false;
            points.removeFirst();
            peak.setX(point.x());
            peak.setY(point.y());

            index_peak=0;

            valley.setX(point.x());
            valley.setY(point.y());
            valley=point;
            index_valley=0;
        }
        points<<point;
        if(point.y()>peak.y())
        {
            peak.setY(point.y());
            index_peak=points.size()-1;
        }
        if(point.y()<valley.y())
        {
            valley.setY(point.y());
            index_valley=points.size()-1;
        }

        ui->peaklabel->setText(QString("( %1 , %2 )").arg(peak.x()).arg(peak.y()));
        ui->valleylabel->setText(QString("( %1 , %2 )").arg(valley.x()).arg(valley.y()));

        curve2->setSamples(points);
        curve2->attach(ui->qwtPlot);
        curve2->setLegendAttribute(curve2->LegendShowLine);

    }
}

void MainWindow::stopThread()
{
    //停止线程，不要用terminate();
    thread->quit();
    //等待线程关闭
    thread->wait();
    qDebug()<<"stop";
}

//------------------------------------------------------------------------------------------------>保护成员，widget事件

void MainWindow::paintEvent(QPaintEvent *)
{
    paint();
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{

    QPointF point = event->pos();//窗口内位置
    //qDebug()<<"("<<point.x()<<","<<point.y()<<")";
    if(ui->qwtPlot->geometry().contains(ui->qwtPlot->mapFrom(this,this->mapFromGlobal(QCursor::pos()))))//注意这里的坐标转换
    {
        globalpoint=point;
    }
}

void MainWindow:: mouseMoveEvent(QMouseEvent *event)
{
    if(ui->qwtPlot->geometry().contains(ui->qwtPlot->mapFrom(this,this->mapFromGlobal(QCursor::pos()))))
    {
        drag_flag=true;

        QPointF point = event->pos();//窗口内位置

        qreal dx=0;

        dx=point.x()-globalpoint.x();

        double move=dx/(qwtw/spanx);
        xmin=xmin-move;
        xmax=xmax-move;
        ui->qwtPlot->setAxisScale(QwtPlot::xBottom,xmin,xmax,accuracyx);
        globalpoint=point;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    timer2_close();
}

void MainWindow:: wheelEvent(QWheelEvent *event)
{
    if(ui->tebWidget->currentIndex()==0)
    {
        if(ui->qwtPlot->geometry().contains(ui->qwtPlot->mapFrom(this,this->mapFromGlobal(QCursor::pos()))))
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
    else if(ui->tebWidget->currentIndex()==2 && wheel_steer_flag && serial.isOpen())
    {
        group_wheel(1,ui->group1,event);
        group_wheel(2,ui->group2,event);
        group_wheel(3,ui->group3,event);
        group_wheel(4,ui->group4,event);
        group_wheel(5,ui->group5,event);
        group_wheel(6,ui->group6,event);
        group_wheel(7,ui->group7,event);
        group_wheel(8,ui->group8,event);
        group_wheel(9,ui->group9,event);
        group_wheel(10,ui->group10,event);

    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
//    if(event->text()=="\r")
//    {
//        if(ui->tebWidget->currentIndex()==1)
//        {
//            this->on_send_Button_clicked();
//        }
//    }
}

void MainWindow:: timerEvent(QTimerEvent *e)
{
    if(e->timerId()==timerID)
    {
        timecount++;
        ui->label->setText(QString::number(timecount/100.0,'f',2));
        setgoal();
    }
    else if(e->timerId()==timerID2)
    {
        if(delay)
        {
            delay--;
            return;
        }
        group_find();
    }
    else if(e->timerId()==timerID3)
    {
        button= qApp->mouseButtons();
        if(bandratebox_index==ui->bandRateBox->currentIndex() && button==Qt::RightButton )
        {
            bandrate_delete(bandratebox_index);
        }

    }
}






















































