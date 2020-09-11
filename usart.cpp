#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mythread.h"
#include "usart.h"

//------------------------------------------------------------------------------------------------>自定义函数

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

    ui->ReceiveEdit->setFont(QFont(tr("Consolas"),10));
    ui->ReceiveEdit->setReadOnly(true);
    ui->SendEdit->setFont(QFont(tr("Consolas"),10));

    ui->dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
}

bool MainWindow::reverseUSART()
{
    if(ui->open_Button->text() == QString("打开串口"))
    {
        //设置串口端口号
        serial.setPortName(comname.value(ui->comBox->currentIndex()));


        qDebug()<<ui->comBox->currentIndex()<<comname.value(ui->comBox->currentIndex())<<"被打开";//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


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

void MainWindow:: receive()
{

    static QByteArray receive;

    //读取串口收到的数据
    QByteArray buffer = serial.readAll();
    qDebug()<<"buffer ="<<buffer;
    if(ui->tebWidget->currentIndex()==2)
    {

        char* byte=buffer.right(1).data();
        if(byte[0]==0x60 && (download_flag==true || part_download_flag==true || repeat_flag==true))
        {
            this->download_data();
            qDebug()<<download_flag<<part_download_flag<<repeat_flag;
            return;
        }
        else if(byte[0]==0x60 && download_flag==false && part_download_flag==true && repeat_flag==false)
        {
            this->download_data();
            return;
        }
        else if(byte[0]==0x60 && download_flag==false && part_download_flag==false && repeat_flag==true)
        {
            this->download_data();
            return;
        }
    }

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
       else
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



//------------------------------------------------------------------------------------------------>私有槽函数

void MainWindow::on_clear_r_Button_clicked()
{
    ui->ReceiveEdit->clear();
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


//------------------------------------------------------------------------------------------------>Event事件

//以下部分主要使用mfc，具体请参考msdn
//热插拔事件
bool MainWindow:: nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    MSG* msg = reinterpret_cast<MSG*>(message);
    int msgType = msg->message;
    if(msgType == WM_DEVICECHANGE)
    {
        //qDebug() << "Recv Event " ;
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
                int i=0;
                comname.clear();
                ui->comBox->clear();
                foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
                {
                    QString item;
                    item=info.portName()+" "+info.description();
                    ui->comBox->addItem(item);
                    comname<<info.portName();
                    i++;
                }
            }
            qDebug() << "usb插入" ;
            break;
        case DBT_DEVICEREMOVECOMPLETE:  //当有usb拔出时
            QString current_com=ui->comBox->currentText();
            if (lpdb -> dbch_devicetype == DBT_DEVTYP_PORT) //筛选是否为串口
            {
                int i=0;
                comname.clear();
                ui->comBox->clear();
                foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
                {
                    QString item;
                    item=info.portName()+" "+info.description();
                    ui->comBox->addItem(item);
                    comname<<info.portName();
                    //qDebug()<<i<<comname.value(i)<<item;
                    i++;
                }
                if(-1==ui->comBox->findText(current_com))
                {
                    if(ui->open_Button->text() == QString("关闭串口"))
                    {
                        reverseUSART();
                    }
                }
            }
            qDebug() << "usb拔出" ;
            break;
        }
    }
    return false;
}

void MainWindow::on_ReceiveEdit_textChanged()
{
    ui->ReceiveEdit->moveCursor(QTextCursor::End);

}

void MainWindow::on_SendEdit_textChanged()
{
    if(ui->SendEdit->toPlainText().right(1)=="\n")
    {
        ui->SendEdit->setText(ui->SendEdit->toPlainText().left(ui->SendEdit->toPlainText().length()-1));

        this->on_send_Button_clicked();

        QTextCursor cursor=ui->SendEdit->textCursor();

        cursor.setPosition(ui->SendEdit->toPlainText().length());
        ui->SendEdit->setTextCursor(cursor);
    }

}
