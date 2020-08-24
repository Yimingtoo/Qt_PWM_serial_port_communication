#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mythread.h"

#include "usart.h"

#include <QDebug>
#include <QMessageBox>
#include <QPointF>
#include <QTextEdit>

void MainWindow::usart_init()
{
    int i=0;
    comname.clear();
    //搜索端口号
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QString item;
        item=info.portName()+" "+info.description();
        ui->comBox->addItem(item);
        comname<<info.portName();
        qDebug()<<i<<comname.value(i)<<item;
        i++;
    }

    ui->ReceiveEdit->setFont(QFont(tr("Source Code Pro"),9));
    ui->dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    //timerID2=this->startTimer(1000);
}


void MainWindow:: receive()
{
    static QByteArray receive;

    //读取串口收到的数据
    QByteArray buffer = serial.readAll();

 //   qDebug()<<buffer;//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    //判断是否需要16进制显示
    if(ui->tebWidget->currentIndex()==1)
    {
       if(ui->show16Box->isChecked()==true)
       {
           buffer = buffer.toHex() ;//转换为16进制 例："1234" -->“31323334”
           thread->setflag(REC_FLAG);
           thread->setbuff(buffer);
           thread->start();
       }
       ui->ReceiveEdit->insertPlainText((QString) buffer);
    }
    else if(ui->tebWidget->currentIndex()==0)
    {
        thread->setflag(DRAW_FLOAT_FLAG);
        thread->setbuff(buffer);
        thread->settime(ui->label->text().toDouble());
        thread->start();
    }
}

float MainWindow::float_rec(QByteArray buffer)
{
    QByteArray receive=buffer.left(5).right(4);
    char* buff=receive.data();
    float data=*((float*)buff);


    qDebug()<<data;//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


    return data;
}

bool MainWindow::reverseUSART()
{
    if(ui->open_Button->text() == QString("打开串口"))
    {
        //设置串口端口号
        serial.setPortName(comname.value(ui->comBox->currentIndex()));
        qDebug()<<ui->comBox->currentIndex()<<comname.value(ui->comBox->currentIndex());
        //设置波特率
        serial.setBaudRate(ui->bandRateBox->currentText().toInt());
        //设置数据位
        switch (ui->dataBitBox->currentText().toInt())
        {
            case 8: serial.setDataBits(QSerialPort::Data8); break;
            case 7: serial.setDataBits(QSerialPort::Data7); break;
            case 6: serial.setDataBits(QSerialPort::Data6); break;
            case 5: serial.setDataBits(QSerialPort::Data5); break;
            default: break;
        }
        //设置停止位
        switch (ui->stopBitBox->currentText().toInt())
        {
            case 1: serial.setStopBits(QSerialPort::OneStop);break;
            case 2: serial.setStopBits(QSerialPort::TwoStop);break;
            default:break;
        }
        //设置校验方式
        switch (ui->checkoutBox->currentIndex())
        {
            case 0: serial.setParity(QSerialPort::NoParity);break;
            default:break;
        }

        //设置流控制模式
        serial.setFlowControl(QSerialPort::NoFlowControl);

        if(serial.open(QIODevice::ReadWrite) == false)
        {
            QMessageBox::warning(NULL,"Warning","串口打开失败！");
            return false;
        }
        //失能串口设置控件
        ui->comBox->setEnabled(false);
        ui->checkoutBox->setEnabled(false);
        ui->bandRateBox->setEnabled(false);
        ui->dataBitBox->setEnabled(false);
        ui->stopBitBox->setEnabled(false);
        //调整文字
        ui->open_Button->setText(QString("关闭串口"));

    }
    else //关闭串口
    {
        serial.close();
        // 使能串口设置控件
        ui->comBox->setEnabled(true);
        ui->checkoutBox->setEnabled(true);
        ui->bandRateBox->setEnabled(true);
        ui->dataBitBox->setEnabled(true);
        ui->stopBitBox->setEnabled(true);
        //调整串口控制按钮的文字提示
        ui->open_Button->setText(QString("打开串口"));
    }
    return true;
}

void MainWindow::on_sendPIDButton_clicked()
{


}

void MainWindow::on_send_Button_clicked()
{
    if(serial.isOpen() == false)
    {
        QMessageBox::warning(NULL,"Warning","请打开串口！");
        return;
    }
    //将内容转化为纯文本，再转化为UTF_8
    QByteArray senddata = ui->SendEdit->toPlainText().toUtf8();

    //判断是否有非16进制字符
    if(ui->send16Box->isChecked()==true) //勾选了16进制发送
     {

        int cnt = senddata.size();          //要发送数据的长度

        char *data = senddata.data();
        for(int i=0;i<cnt;i++)//判断是否有非16进制字符
        {
            if(data[i]>='0' && (data[i]<='9'))
                continue;
            else if(data[i]>='a' && (data[i]<='f'))
                continue;
            else if(data[i]>='A' && (data[i]<='F'))
                continue;
            else if(data[i] == ' ')     //输入为空格
                continue;
            else
            {
                QMessageBox::warning(NULL , "提示", "输入非16进制字符！");
                return;
            }
        }

        //字符串转化为16进制数   "1234" --> 0X1234
        //转换时会自动除去非16进制字符
        senddata = senddata.fromHex(senddata);
    }
    serial.write(senddata);
}


void MainWindow::on_open_Button_clicked()
{
    this->reverseUSART();
}


void MainWindow::on_clear_r_Button_clicked()
{
    ui->ReceiveEdit->clear();
}
