#include "mainwindow.h"
#include "ui_mainwindow.h"

//------------------------------------------------------------------------------------------------>自定义函数

void MainWindow::draw_init()
{
    drag_flag=false;
    timerIsRun=false;
    fix_flag=false;
    firstpoint_flag=true;
    isPIDopen_flag=false;

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

    ui->yminSpinBox->setMinimum(-100000);
    ui->ymaxSpinBox->setMinimum(-100000);

    goalpoints<<QPointF(0,0)<<QPointF(0,0);

    target_value=0;

    draw();

    curve1=new QwtPlotCurve("curve1");
    curve1->setPen(Qt::green,2);//设置曲线为蓝色，线粗为2
    curve1->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化

    ui->pSpinBox->setSingleStep(0.1);
    ui->iSpinBox->setSingleStep(0.1);
    ui->dSpinBox->setSingleStep(0.1);
    ui->goalSpinBox->setSingleStep(0.1);

    PID_setvisible(false);
}

void MainWindow::draw()
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

void MainWindow::paint()
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
    if(drag_flag==false)//如果移动则停止自动移动
    {
        bool ok;
        if(ui->label->text().toDouble(&ok)-xmax>5)
        {
            xmin=xmin+ui->label->text().toDouble(&ok)-xmax+1;
            xmax=xmax+ui->label->text().toDouble(&ok)-xmax+1;

            ui->qwtPlot->setAxisScale(QwtPlot::xBottom,xmin,xmax,accuracyx);
        }
        if(ui->label->text().toDouble(&ok)>xmax-1)
        {
            xmax+=0.04;
            xmin+=0.04;
            ui->qwtPlot->setAxisScale(QwtPlot::xBottom,xmin,xmax,accuracyx);
        }

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
        if(points.at(points.size()-1).y()>ymax-1 || goalpoints.at(goalpoints.size()-1).y()>ymax-1)
        {
            ymax=points.at(points.size()-1).y()>goalpoints.at(goalpoints.size()-1).y()?points.at(points.size()-1).y():goalpoints.at(goalpoints.size()-1).y()+1;
        }
        if(points.at(points.size()-1).y()<ymin+1 || goalpoints.at(goalpoints.size()-1).y()<ymin-1)
        {
            ymin=points.at(points.size()-1).y()<goalpoints.at(goalpoints.size()-1).y()?points.at(points.size()-1).y():goalpoints.at(goalpoints.size()-1).y()-1;
        }

        spany=ymax-ymin;
        accuracyy=int(60*spany/qwth);
        ui->qwtPlot->setAxisScale(QwtPlot::yLeft,ymin,ymax,accuracyy);
    }
    else
    {
        if(ui->ymaxSpinBox->value()-ui->yminSpinBox->value()<=0)
        {
            if(fix_ymax!=ui->ymaxSpinBox->value())
            {
                ui->ymaxSpinBox->setValue(ui->yminSpinBox->value());
            }
            if(fix_ymin!=ui->yminSpinBox->value())
            {
                ui->yminSpinBox->setValue(ui->ymaxSpinBox->value());
            }
            return;
        }
        fix_ymax=ui->ymaxSpinBox->value();
        fix_ymin=ui->yminSpinBox->value();
        spany=fix_ymax-fix_ymin;


        accuracyy=int(60*spany/qwth);
        ui->qwtPlot->setAxisScale(QwtPlot::yLeft,fix_ymin,fix_ymax,accuracyy);
    }
}

void MainWindow::setgoal()
{
    if(isPIDopen_flag)
    {
        if(target_value!=goalpoints.value(goalpoints.size()-1).y())
        {
            goalpoints<<QPointF(timecount/100.0,target_value);
            goalpoints<<QPointF(timecount/100.0,target_value);
        }
        else
        {
            goalpoints.replace(goalpoints.size()-1,QPointF(timecount/100.0,target_value));
        }
        curve1->setSamples(goalpoints);
        curve1->attach(ui->qwtPlot);
        curve1->setLegendAttribute(curve1->LegendShowLine);
    }
}

void MainWindow::PID_setvisible(bool ok)
{
    ui->pSpinBox->setVisible(ok);
    ui->plabel->setVisible(ok);
    ui->iSpinBox->setVisible(ok);
    ui->ilabel->setVisible(ok);
    ui->dSpinBox->setVisible(ok);
    ui->dlabel->setVisible(ok);
    ui->sendPIDButton->setVisible(ok);
    ui->goalSpinBox->setVisible(ok);
    ui->goallabel->setVisible(ok);
    ui->setgoalButton->setVisible(ok);
}

//------------------------------------------------------------------------------------------------>私有槽函数

void MainWindow::on_startButton_clicked()
{
    if(this->reverseUSART())
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

void MainWindow::on_restorationButton_clicked()
{
    drag_flag=false;
    spanx=qwtw/36;
    xmin=points.at(points.size()-1).x()-2;
    xmax=xmin+spanx;

    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,xmin,xmax,accuracyx);
}

void MainWindow::on_fixButton_clicked()
{
    if(fix_flag==false && ui->ymaxSpinBox->value()-ui->yminSpinBox->value()>0)
    {
        fix_flag=true;
        ui->fixButton->setText("解固");
    }
    else if(ui->ymaxSpinBox->value()-ui->yminSpinBox->value()<=0)
    {
        QMessageBox *msgdlg=new QMessageBox;
        msgdlg->setWindowModality(Qt::NonModal);
        msgdlg->setWindowTitle("警告");
        msgdlg->setText("Ymax与Ymin的差值必须大于零");
        msgdlg->show();
        return;
    }
    else
    {
        fix_flag=false;
        ui->fixButton->setText("固定");
    }
}

void MainWindow::on_resetButton_clicked()
{
    //重置拖拽标志
    drag_flag=false;

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

    //清除数据点
    points.clear();
    points<<QPointF(0,0);
    goalpoints.clear();
    goalpoints<<QPointF(0,0)<<QPointF(0,0);

    //重置画图
    ui->label->setNum(0);
    curve2->setSamples(points);
    curve2->attach(ui->qwtPlot);
    curve2->setLegendAttribute(curve2->LegendShowLine);

    curve1->setSamples(goalpoints);
    curve1->attach(ui->qwtPlot);
    curve1->setLegendAttribute(curve2->LegendShowLine);

    //重置清除第一点标志位
    firstpoint_flag=true;

    //设置按键文本
    ui->startButton->setText("开始");

    //重置峰谷点
    ui->valleylabel->setText("( 0 , 0 )");
    ui->peaklabel->setText("( 0 , 0 )");

}

void MainWindow::on_sendPIDButton_clicked()
{

}

void MainWindow::on_setgoalButton_clicked()
{
    target_value=ui->goalSpinBox->value();
}
