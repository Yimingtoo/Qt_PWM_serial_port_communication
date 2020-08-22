#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSerialPort>//提供访问串口的功能
#include <QSerialPortInfo>//提供系统中存在的串口信息

#include "usart.h"

#include "windows.h"
#include "dbt.h"


#include<qwt_plot.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>

#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_column_symbol.h>
#include <qwt_series_data.h>
#include <qpen.h>
#include <qwt_symbol.h>
#include <qwt_picker_machine.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void test1();

    void draw_init();

    //------------------------>串口

    bool openUSART();//打开串口函数

    void receive();

    void usart_init();

    float float_rec(QByteArray buffer);

private slots:

    void on_pushButton_2_clicked();

    void on_startButton_clicked();

    void on_sendPIDButton_clicked();

    void on_open_Button_clicked();

    //------------------------>串口

    void on_clear_r_Button_clicked();

    void on_send_Button_clicked();

    void on_resetButton_clicked();

    void on_fixButton_clicked();

private:

    Ui::MainWindow *ui;

    double xmax,xmin,ymax,ymin;

    double fix_ymax,fix_ymin;

    bool drag_flag;//坐标轴是否被拖动标志
    bool firstpoint_flag;//第一点标志位
    bool fix_flag;

    QPointF globalpoint;//窗口内位置

    int qwth;//坐标面的高
    int qwtw;//宽

    QRect rect;//qwt的长宽及坐标

    QwtInterval tempIntervalX;//获得刻度所必须

    double spanx;//刻度长度
    double spany;

    QwtPlotCurve *curve2;
    QPolygonF points;//输入节点数据QPointF(x,y)

    int accuracyx;//刻度的精度
    int accuracyy;

    int timerID;
    int timerID2;
    bool timerIsRun;
    int timecount;

    QPointF peak,valley;//图像峰值和谷值
    int index_peak,index_valley;

    //------------------------>串口

    QSerialPort serial;
    QSerialPortInfo info_serial;

protected:

    //绘图必须在绘图时间中实现
    //绘图事件系统自动调用
    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    //定时器事件

    void timerEvent(QTimerEvent *e);

    bool nativeEvent(const QByteArray &eventType, void *message, long *result);


};
#endif // MAINWINDOW_H
