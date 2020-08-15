#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "usart.h"

#include <QDebug>
#include <QMessageBox>
#include <QPointF>

void MainWindow::usart_init()
{

    //搜索端口号
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comBox->addItem(info.portName());
    }
}


void MainWindow:: receive()
{
    static QByteArray receive;
    //读取串口收到的数据

    QByteArray buffer = serial.readAll();
    bool ok;

    qDebug()<<buffer;//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


    //判断是否需要16进制显示
    if(ui->show16Box->isChecked()==true)
    {
        buffer = buffer.toHex() ;//转换为16进制 例："1234" -->“31323334”
        ui->ReceiveEdit->insertPlainText((QString) buffer);
        qDebug()<<buffer;
    }
    else
    {
        switch(buffer.left(1).toHex().toInt(&ok,16))
        {
            case REC_FLOAT:
                float temp=float_rec(buffer);
                float temp_time=ui->label->text().toDouble();

                if(firstpoint_flag)
                {
                    firstpoint_flag=false;
                    points.removeFirst();

                    peak.setX(temp_time);
                    peak.setY(temp);
                    index_peak=points.size()-1;

                    valley.setX(temp_time);
                    valley.setY(temp);
                    index_valley=points.size()-1;

                    ymax=temp;
                    ymin=temp;

                }

                points<<QPointF(temp_time,temp);

                if(temp>peak.y())
                {
                    peak.setX(temp_time);
                    peak.setY(temp);
                    index_peak=points.size()-1;
                    ymax=temp;
                }
                if(temp<valley.y())
                {
                    valley.setX(temp_time);
                    valley.setY(temp);
                    index_valley=points.size()-1;
                    ymin=temp;
                }

                ui->peaklabel->setText(QString("( %1 , %2 )").arg(peak.x()).arg(peak.y()));
                ui->valleylabel->setText(QString("( %1 , %2 )").arg(valley.x()).arg(valley.y()));

                curve2->setSamples(points);
                curve2->attach(ui->qwtPlot);
                curve2->setLegendAttribute(curve2->LegendShowLine);
                break;
        }
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

bool MainWindow::openUSART()
{
    if(ui->open_Button->text() == QString("打开串口"))
    {
        //设置串口端口号
        serial.setPortName(ui->comBox->currentText());
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
        ui->search_Button->setEnabled(false);
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
        ui->search_Button->setEnabled(true);
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
    this->openUSART();
}

void MainWindow::on_search_Button_clicked()
{
    //清除下拉菜单的内容
    ui->comBox->clear();
    //搜索端口号
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comBox->addItem(info.portName());
    }
}

void MainWindow::on_clear_r_Button_clicked()
{
    ui->ReceiveEdit->clear();
}
