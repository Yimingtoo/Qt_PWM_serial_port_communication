#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//Qt头文件
#include <QMainWindow>
#include <QSerialPort>//提供访问串口的功能
#include <QSerialPortInfo>//提供系统中存在的串口信息
#include <QPainter>
#include <QLineF>
#include <QPointF>
#include <QDebug>
#include <QMouseEvent>
#include <QTimer>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFile>
#include <QTextEdit>
#include <QObjectPicker>
#include <QGroupBox>
#include <QDialog>
#include <QFileDialog>
#include <QMenu>
#include <QAction>

//Windows 相关头文件
#include "windows.h"
#include "dbt.h"

//Qwt头文件
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

//自定义头文件
#include "usart.h"
#include "mythread.h"
#include "setting.h"

#define TIME_GROUP(group,dir) group*dir
#define FORWARD 1
#define BACK -1


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public://------------------------------------------------------------------------->共有成员

    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void MainWindow_init();

    //------------------------------------------------->绘图

    void draw_init();

    void draw();

    void paint();

    void setgoal();

    void PID_setvisible(bool ok);

    //------------------------------------------------->串口

    void usart_init();

    bool reverseUSART();//打开串口函数

    float float_rec(QByteArray buffer);

    //------------------------------------------------->波特率

    void bandrate_init();

    void bandrate_delete(int index);

    void bandrate_destruct();

    //------------------------------------------------->舵机控制

    void receive();

    void steering_gear_control_init();

    void write_resetvalue();

    void read_resetvalue();

    QString file_search(QString aim,QString search);

    void group_find();

    void group_wheel(int group,QGroupBox *box,QWheelEvent *event);

    void timer2_open();

    void timer2_close();

    void buttonpressed_deal(int index,int dir);

    void usart_send(int i,char cmd);

    unsigned char find_pinID(QString pin);

    void download_data();

    //------------------------------------------------->槽函数

    void dealThread(QByteArray buffer);

    void stopThread();

    void tabchange();

    void dealfloatThread(QPointF point);

    void pinsettingrec(QString *title);

    void wheel_open_rec(bool isok);

    void PID_open_rec(bool isok);

    void bandrate_rec(QString bandrate);

    void bandrateindex_ischanged(int index);

    void bandrate_ishighlighted(int index);

    void saveas_action_res();

    void writereset_action_res();

    void slotActionInsert();

    void slotActionDelete();

    void debug_save_action_rec();

    void debug_save_as_action_rec();

    void open_action_rec();

    void open_from_action_rec();

private slots://------------------------------------------------------------------------->私有槽函数

    //------------------------------------------------->绘图

    void on_startButton_clicked();

    void on_restorationButton_clicked();

    void on_fixButton_clicked();

    void on_resetButton_clicked();

    void on_sendPIDButton_clicked();

    //------------------------------------------------->串口

    void on_clear_r_Button_clicked();

    void on_send_Button_clicked();

    void on_open_Button_clicked();

    //------------------------------------------------->舵机控制

    void on_resetButton_4_clicked();

    void on_ReceiveEdit_textChanged();

    void on_forwardButton1_clicked();
    void on_forwardButton1_pressed();

    void on_reverseButton1_clicked();
    void on_reverseButton1_pressed();

    void on_forwardButton2_clicked();
    void on_forwardButton2_pressed();

    void on_reverseButton2_clicked();
    void on_reverseButton2_pressed();


    void on_forwardButton3_clicked();
    void on_forwardButton3_pressed();

    void on_reverseButton3_clicked();
    void on_reverseButton3_pressed();


    void on_forwardButton4_clicked();
    void on_forwardButton4_pressed();

    void on_reverseButton4_clicked();
    void on_reverseButton4_pressed();


    void on_forwardButton5_clicked();
    void on_forwardButton5_pressed();

    void on_reverseButton5_clicked();
    void on_reverseButton5_pressed();


    void on_forwardButton6_clicked();
    void on_forwardButton6_pressed();

    void on_reverseButton6_clicked();
    void on_reverseButton6_pressed();


    void on_forwardButton7_clicked();
    void on_forwardButton7_pressed();

    void on_reverseButton7_clicked();
    void on_reverseButton7_pressed();


    void on_forwardButton8_clicked();
    void on_forwardButton8_pressed();

    void on_reverseButton8_clicked();
    void on_reverseButton8_pressed();

    void on_forwardButton9_clicked();
    void on_forwardButton9_pressed();

    void on_reverseButton9_clicked();
    void on_reverseButton9_pressed();

    void on_forwardButton10_clicked();
    void on_forwardButton10_pressed();

    void on_reverseButton10_clicked();
    void on_reverseButton10_pressed();

    void on_setgoalButton_clicked();

    void on_SendEdit_textChanged();


    void on_generateButton_clicked();

    void on_codetableWidget_cellDoubleClicked(int row, int column);

    void on_clearButton_clicked();

    void on_downloadButton_clicked();

    void on_codetableWidget_itemChanged(QTableWidgetItem *item);

    void on_codetableWidget_cellChanged(int row, int column);

    void on_codetableWidget_cellPressed(int row, int column);

    void on_codetableWidget_customContextMenuRequested(const QPoint &pos);


    void on_repeat_download_Button_clicked();

    void on_part_down_Button_clicked();

private://------------------------------------------------------------------------->私有成员

    Ui::MainWindow *ui;

    //------------------------------------------------->系统

    int timerID;
    int timerID2;
    int timerID3;

    Qt::MouseButtons button;

    Setting *setting;

//    Message *msgDlg;

    //------------------------------------------------->线程

    Mythread *thread;

    //------------------------------------------------->绘图

    double xmax,xmin,ymax,ymin;

    double fix_ymax,fix_ymin;

    QPointF globalpoint;//窗口内位置

    int qwth;//坐标面的高
    int qwtw;//宽

    QRect rect;//qwt的长宽及坐标

    QwtInterval tempIntervalX;//获得刻度所必须

    double spanx;//刻度长度
    double spany;

    QwtPlotCurve *curve1;
    QwtPlotCurve *curve2;
    QPolygonF goalpoints;
    QPolygonF points;//输入节点数据QPointF(x,y)

    int accuracyx;//刻度的精度
    int accuracyy;

    int timecount;

    QPointF peak,valley;//图像峰值和谷值
    int index_peak,index_valley;

    float target_value;

    //------------------------------------------------->串口

    QSerialPort serial;
    QSerialPortInfo info_serial;

    QStringList comname;


    int bandratebox_index;

    //------------------------------------------------->舵机控制

    int group_find_value;

    int delay;

    QLabel *dutylabel[10];
    QLabel *varlabel[10];
    QGroupBox *group[10];

    QStringList pinlist;
    QStringList varlist;
    QStringList aimlist;

    QString tabledata;

    QMenu *table_widget_menu;

    int pressed_row;

    int min_row;
    int max_row;

    int code_count;
    bool stop_flag;
    bool is_usr_change_flag;
    bool download_flag;
    bool part_download_flag;
    bool repeat_flag;

    //------------------------------------------------->设置flag

    bool wheel_steer_flag;

    //定时器相关flag

    bool timerIsRun;
    bool timer2IsRun;
    bool timer3IsRun;

    //绘图相关flag

    bool drag_flag;//坐标轴是否被拖动标志
    bool firstpoint_flag;//第一点标志位
    bool fix_flag;
    bool isPIDopen_flag;

protected://------------------------------------------------------------------------->保护成员

    //绘图必须在绘图时间中实现
    //绘图事件系统自动调用
    void paintEvent(QPaintEvent *);

    //鼠标事件
    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    //键盘事件
    void keyPressEvent(QKeyEvent *event);

    //定时器事件
    void timerEvent(QTimerEvent *e);

    //热插拔检测
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);


};
#endif // MAINWINDOW_H
