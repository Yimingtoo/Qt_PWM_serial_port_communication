#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::steering_gear_control_init()
{
    dutylabel[0]=ui->dutylabel1;
    dutylabel[1]=ui->dutylabel2;
    dutylabel[2]=ui->dutylabel3;
    dutylabel[3]=ui->dutylabel4;
    dutylabel[4]=ui->dutylabel5;
    dutylabel[5]=ui->dutylabel6;
    dutylabel[6]=ui->dutylabel7;
    dutylabel[7]=ui->dutylabel8;
    dutylabel[8]=ui->dutylabel9;
    dutylabel[9]=ui->dutylabel10;

    varlabel[0]=ui->variationlabel1;
    varlabel[1]=ui->variationlabel2;
    varlabel[2]=ui->variationlabel3;
    varlabel[3]=ui->variationlabel4;
    varlabel[4]=ui->variationlabel5;
    varlabel[5]=ui->variationlabel6;
    varlabel[6]=ui->variationlabel7;
    varlabel[7]=ui->variationlabel8;
    varlabel[8]=ui->variationlabel9;
    varlabel[9]=ui->variationlabel10;

    group[0]=ui->group1;
    group[1]=ui->group2;
    group[2]=ui->group3;
    group[3]=ui->group4;
    group[4]=ui->group5;
    group[5]=ui->group6;
    group[6]=ui->group7;
    group[7]=ui->group8;
    group[8]=ui->group9;
    group[9]=ui->group10;

    QFile file("PinSetting.dat");
    bool res=file.open(QIODevice::ReadWrite);
    if(false==res)
    {
        qDebug()<<"打开IARpath.txt失败\n";
        return;
    }
    QString pin_str=file.readAll();
    for(int i=0;i<10;i++)
    {
        group[i]->setTitle(pin_str.left(3));
        group[i]->setStyleSheet("QGroupBox  {color: blue;}");
        pin_str=pin_str.remove(0,4);
    }

    ui->velocityBox->setMaximum(50);
    ui->delayBox->setMaximum(10000);

    read_resetvalue();

    wheel_steer_flag=0;
    code_count=false;
    stop_flag=false;
    restart_flag=true;
    //代码表格

    ui->codetableWidget->setColumnCount(3);//设置列数

    ui->codetableWidget->setHorizontalHeaderLabels(QStringList()<<"引脚号"<<"增量"<<"目标量");//设置水平表头
    ui->codetableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->codetableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->codetableWidget->setColumnWidth(1,40);
    ui->codetableWidget->setColumnWidth(0,70);
    ui->codetableWidget->setColumnWidth(2,70);

}

void MainWindow::saveas_action_res()
{
    bool ok;
    QString file_path=QFileDialog::getOpenFileName(this,"工程文件路径","../","prj(*.prj);;All(*.*)");
    QString prjpath,tempstr;
    for(int i=0;i<file_path.length();i++)
    {
        if(file_path[i]=='/')
        {
            prjpath=prjpath+tempstr+"/";
            tempstr="";
            continue;
        }
        tempstr=tempstr+file_path[i];
    }
    qDebug()<<prjpath;
    QFile file(prjpath+"Ctrl_data.h");

    file.open(QIODevice::WriteOnly);
    file.write(QByteArray(""));
    file.close();

    int listsize=pinlist.size();
    int index[10]={0,0,0,0,0,0,0,0,0,0};
    int cnt=0;
    file.open(QIODevice::Append);

    file.write(QByteArray("#ifndef __CTRL_H\n"));
    file.write(QByteArray("#define __CTRL_H\n"));
//    for(int i=0;i<listsize;i++)
//    {
//        for(int j=0;j<10;j++)
//        {
//            if(group[j]->title()==pinlist[i])
//            {
//                index[cnt]=j;
//                cnt++;
//            }
//        }
//    }
//    file.write(QString("#define SERVO_NUM_FOR_USER %1\n").arg(cnt).toUtf8());
//    for(int i=0;i<10;i++)
//    {
//        for(int j=0;j<cnt;j++)
//        {
//            if(i==index[j])
//            {
//                file.write((QString("#define Part0 ") + group[i]->title()+"\n").toUtf8());
//            }
//        }
//    }



    file.write(QString("unsigned int workdata[%1] =\n").arg(listsize).toUtf8());
        file.write(QByteArray("{\n"));
        for(int i=0;i<listsize-1;i++)
        {
            unsigned char pin_id=find_pinID(pinlist[i]);
            if(pin_id==0xff)
                return;
            unsigned char temphigh=(unsigned char)(aimlist[i].toInt(&ok)>>8);
            unsigned char templow=(unsigned char)(aimlist[i].toInt(&ok));
            unsigned int data=((unsigned int)(temphigh|pin_id)<<8)+templow;
            qDebug()<<data;
            file.write(QString("0x%1,\t").arg(data,4,16,QLatin1Char('0')).toUtf8());
            if(i%8==7)
            {
                file.write(QByteArray("\n"));
            }
        }
            file.write(QByteArray("0x7374"));
        file.write(QByteArray("\n};\n"));
    file.write(QByteArray("#endif\n"));

    file.close();
}

void MainWindow::pinsettingrec(QString *title)
{
    for(int i=0;i<10;i++)
    {
        group[i]->setTitle(title[i]);
    }
    write_resetvalue();
}

void MainWindow::write_resetvalue()
{
    QString title[10],write;
    for(int i=0;i<10;i++)
    {
        title[i] = group[i]->title();
        title[i] = title[i]+" "+dutylabel[i]->text();
    }

    QFile file("resetvalue.dat");
    bool res=file.open(QIODevice::WriteOnly);
    if(false==res)
    {
        qDebug()<<"打开resetvalue.dat失败\n";
        return;
    }
    for(int i=0;i<10;i++)
        write=write+title[i]+"\n";
    file.write(write.toUtf8());
    file.close();
}

QString MainWindow::file_search(QString aim,QString search)
{
    QString value;
    while(search!="\0")
    {
        if(aim==search.left(3))
        {
            search=search.remove(0,4);
            while(search.left(1)!="\n")
            {
                value=value+search.left(1);
                search=search.remove(0,1);
            }
        }
        search=search.remove(0,1);
    }
    return value;
}

void MainWindow::read_resetvalue()
{
    QFile file("resetvalue.dat");
    bool res=file.open(QIODevice::ReadWrite);
    if(false==res)
    {
        qDebug()<<"打开resetvalue.dat失败\n";
        return;
    }
    QString read_str = file.readAll();

    for(int i=0;i<10;i++)
    {
        dutylabel[i]->setText(file_search(group[i]->title(),read_str));
    }
}

unsigned char MainWindow::find_pinID(QString pin)
{
    unsigned char pin_id;
    if(pin=="PA0")pin_id=0x00;
    else if(pin=="PA1")pin_id=0x04;
    else if(pin=="PA2")pin_id=0x08;
    else if(pin=="PA3")pin_id=0x0c;
    else if(pin=="PA4")pin_id=0x10;
    else if(pin=="PA5")pin_id=0x14;
    else if(pin=="PA6")pin_id=0x18;
    else if(pin=="PA7")pin_id=0x1c;

    else if(pin=="PB0")pin_id=0x20;
    else if(pin=="PB1")pin_id=0x24;
    else if(pin=="PB2")pin_id=0x28;
    else if(pin=="PB3")pin_id=0x2c;
    else if(pin=="PB4")pin_id=0x30;
    else if(pin=="PB5")pin_id=0x34;
    else if(pin=="PB6")pin_id=0x38;
    else if(pin=="PB7")pin_id=0x3c;

    else if(pin=="PC0")pin_id=0x40;
    else if(pin=="PC1")pin_id=0x44;
    else if(pin=="PC2")pin_id=0x48;
    else if(pin=="PC3")pin_id=0x4c;
    else if(pin=="PC4")pin_id=0x50;
    else if(pin=="PC5")pin_id=0x54;
    else if(pin=="PC6")pin_id=0x58;
    else if(pin=="PC7")pin_id=0x5c;

    else if(pin=="PD0")pin_id=0x60;
    else if(pin=="PD1")pin_id=0x64;
    else if(pin=="PD2")pin_id=0x68;
    else if(pin=="PD3")pin_id=0x6c;
    else if(pin=="PD4")pin_id=0x70;
    else if(pin=="PD5")pin_id=0x74;
    else if(pin=="PD6")pin_id=0x78;
    else if(pin=="PD7")pin_id=0x7c;
    else if(pin=="Speed")pin_id=0xc8;
    else if(pin=="Delay")pin_id=0xc4;
    else if(pin=="")pin_id=0xfc;
    else pin_id=0xff;

    return pin_id;
}

void MainWindow::on_downloadButton_clicked()
{
    bool ok;

    char cmd=0x60;
    if(restart_flag)
    {
        code_count=0;
        restart_flag=0;
        serial.write(&cmd,1);
        ui->downloadButton->setEnabled(false);
    }
    else
    {
        int i;
        for(i=0;i<10;i++)
        {
            if(group[i]->title()==pinlist[code_count])
                break;
        }

        unsigned char pin_id=find_pinID(pinlist[code_count]);
        if(pin_id==0xff)
            return;
        unsigned char temphigh=(unsigned char)(aimlist[code_count].toInt(&ok)>>8);
        unsigned char templow=(unsigned char)(aimlist[code_count].toInt(&ok));

        if(code_count<pinlist.size()-1)
        {
            char datahigh=(char)(pin_id|temphigh);
            char datalow=(char)templow;
            serial.write(&datahigh,1);
            serial.write(&datalow,1);
            qDebug()<<datahigh<<datalow;
        }
        else
        {
            char datahigh=(char)0x73;
            char datalow=(char)0x74;
            serial.write(&datahigh,1);
            serial.write(&datalow,1);
            code_count=0;
            restart_flag=1;
            ui->downloadButton->setEnabled(true);
             qDebug()<<datahigh<<datalow;
        }
        code_count++;
    }

}

void MainWindow::on_clearButton_clicked()
{
    ui->codetableWidget->setRowCount(0);
    pinlist.clear();
    varlist.clear();
    aimlist.clear();

}

void MainWindow::on_codetableWidget_cellDoubleClicked(int row, int column)
{
    ui->codetableWidget->removeRow(row);
    pinlist.removeAt(row);
    varlist.removeAt(row);
    aimlist.removeAt(row);
}

void MainWindow::on_generateButton_clicked()
{
    int count=0;
    if(ui->velocityBox->value())
    {
        pinlist.append("Speed");
        varlist.append("");
        aimlist.append(QString::number(ui->velocityBox->value()));
        count++;
        ui->velocityBox->setValue(0);
    }
    for(int i=0;i<10;i++)
    {
        if(varlabel[i]->text()=="0")
            continue;
        pinlist.append(group[i]->title());
        varlist.append(varlabel[i]->text());
        aimlist.append(dutylabel[i]->text());
        count++;
    }
    if(ui->delayBox->value())
    {
        pinlist.append("Delay");
        varlist.append("");
        aimlist.append(QString::number(ui->delayBox->value()));
        count++;
        ui->delayBox->setValue(0);
    }

    if(count)
    {
        pinlist.append("");
        varlist.append("");
        aimlist.append("");
        count++;
    }

    int current_row=ui->codetableWidget->rowCount();
    ui->codetableWidget->setRowCount(current_row+count);

    //qDebug()<<current_row<<ui->codetableWidget->rowCount()<<count;



    for(int i=current_row;i<current_row+count;i++)
    {
       // qDebug()<<i;

        int col=0;
        ui->codetableWidget->setItem(i,col++,new QTableWidgetItem(pinlist[i]));
        ui->codetableWidget->setItem(i,col++,new QTableWidgetItem(varlist[i]));
        ui->codetableWidget->setItem(i,col++,new QTableWidgetItem(aimlist[i]));

    }
    ui->codetableWidget->setRowHeight(ui->codetableWidget->rowCount()-1,5);
     ui->codetableWidget->scrollToBottom();  //让滚动条滚动到最底部

     ui->velocityBox->setValue(0);
     ui->delayBox->setValue(0);
     for(int i=0;i<10;i++)
     {
         varlabel[i]->setNum(0);
     }

}

void MainWindow::on_resetButton_4_clicked()
{
    read_resetvalue();

    for(int i=0;i<10;i++)
    {
        varlabel[i]->setNum(0);
    }

    ui->velocityBox->setValue(0);
    ui->delayBox->setValue(0);
    for(int i=0;i<10;i++)
        usart_send(i,0x50);
}

void MainWindow::usart_send(int i, char cmd)
{
//    bool ok;

//    QString varnum=varlabel[i]->text();
//    QString dutynum=dutylabel[i]->text();
//    dutynum=QString::number(dutynum.toInt(&ok),2);
//    int j=10-dutynum.length();
//    for(int i=0;i<j;i++)
//    {
//        dutynum="0"+dutynum;

//    }
//    QString senddatas;
//    senddatas="01010000000000"+dutynum;

//    int num1=senddatas.left(8).toInt(&ok,2);
//    int num2=senddatas.left(16).right(8).toInt(&ok,2);
//    int num3=senddatas.left(24).right(8).toInt(&ok,2);

//    int *pn1=&num1;
//    int *pn2=&num2;
//    int *pn3=&num3;

//    char*pnc1=(char*)pn1;
//    char*pnc2=(char*)pn2;
//    char*pnc3=(char*)pn3;

//    serial.write(pnc1,1);
//    serial.write(pnc2,1);
//    serial.write(pnc3,1);


    bool ok;
    unsigned char pin_id;
    char senddat[4];
    senddat[0]=cmd;
    pin_id=find_pinID(group[i]->title());

    QString dutynum=dutylabel[i]->text();
    unsigned int duty_int=dutynum.toInt(&ok);

    qDebug()<<pin_id;

    unsigned char tempHigh=(unsigned char)(duty_int>>8);
    unsigned char tempLow=(unsigned char)(duty_int);

    senddat[1]=pin_id|tempHigh;
    senddat[2]=tempLow;

    senddat[3]='\0';
    QByteArray byte=senddat;
    qDebug()<<byte;
    serial.write(senddat,3);

}

void MainWindow::group_find()
{
    bool ok;

    switch(group_find_value)
    {
    case TIME_GROUP(1,FORWARD):
        if(ui->dutylabel1->text().toInt(&ok)<600 )
        {
            ui->dutylabel1->setNum(ui->dutylabel1->text().toInt(&ok,10)+1);
            ui->variationlabel1->setNum(ui->variationlabel1->text().toInt(&ok,10)+1);
        }
        break;
    case TIME_GROUP(1,BACK):
        if(ui->dutylabel1->text().toInt(&ok)>0)
        {
            ui->dutylabel1->setNum(ui->dutylabel1->text().toInt(&ok,10)-1);
            ui->variationlabel1->setNum(ui->variationlabel1->text().toInt(&ok,10)-1);
        }
        break;

    case TIME_GROUP(2,FORWARD):
        if(ui->dutylabel2->text().toInt(&ok)<600)
        {
            ui->dutylabel2->setNum(ui->dutylabel2->text().toInt(&ok,10)+1);
            ui->variationlabel2->setNum(ui->variationlabel2->text().toInt(&ok,10)+1);
        }
        break;
    case TIME_GROUP(2,BACK):
        if(ui->dutylabel2->text().toInt(&ok)>0)
        {
            ui->dutylabel2->setNum(ui->dutylabel2->text().toInt(&ok,10)-1);
            ui->variationlabel2->setNum(ui->variationlabel2->text().toInt(&ok,10)-1);
        }
        break;

    case TIME_GROUP(3,FORWARD):
        if(ui->dutylabel3->text().toInt(&ok)<600)
        {
            ui->dutylabel3->setNum(ui->dutylabel3->text().toInt(&ok,10)+1);
            ui->variationlabel3->setNum(ui->variationlabel3->text().toInt(&ok,10)+1);
        }
        break;
    case TIME_GROUP(3,BACK):
        if(ui->dutylabel3->text().toInt(&ok)>0)
        {
            ui->dutylabel3->setNum(ui->dutylabel3->text().toInt(&ok,10)-1);
            ui->variationlabel3->setNum(ui->variationlabel3->text().toInt(&ok,10)-1);
        }
        break;

    case TIME_GROUP(4,FORWARD):
        if(ui->dutylabel4->text().toInt(&ok)<600)
        {
            ui->dutylabel4->setNum(ui->dutylabel4->text().toInt(&ok,10)+1);
            ui->variationlabel4->setNum(ui->variationlabel4->text().toInt(&ok,10)+1);
        }
        break;
    case TIME_GROUP(4,BACK):
        if(ui->dutylabel4->text().toInt(&ok)>0)
        {
            ui->dutylabel4->setNum(ui->dutylabel4->text().toInt(&ok,10)-1);
            ui->variationlabel4->setNum(ui->variationlabel4->text().toInt(&ok,10)-1);
        }
        break;

    case TIME_GROUP(5,FORWARD):
        if(ui->dutylabel5->text().toInt(&ok)<600)
        {
            ui->dutylabel5->setNum(ui->dutylabel5->text().toInt(&ok,10)+1);
            ui->variationlabel5->setNum(ui->variationlabel5->text().toInt(&ok,10)+1);
        }
        break;
    case TIME_GROUP(5,BACK):
        if(ui->dutylabel5->text().toInt(&ok)>0)
        {
            ui->dutylabel5->setNum(ui->dutylabel5->text().toInt(&ok,10)-1);
            ui->variationlabel5->setNum(ui->variationlabel5->text().toInt(&ok,10)-1);
        }
        break;

    case TIME_GROUP(6,FORWARD):
        if(ui->dutylabel6->text().toInt(&ok)<600)
        {
            ui->dutylabel6->setNum(ui->dutylabel6->text().toInt(&ok,10)+1);
            ui->variationlabel6->setNum(ui->variationlabel6->text().toInt(&ok,10)+1);
        }
        break;
    case TIME_GROUP(6,BACK):
        if(ui->dutylabel6->text().toInt(&ok)>0)
        {
            ui->dutylabel6->setNum(ui->dutylabel6->text().toInt(&ok,10)-1);
            ui->variationlabel6->setNum(ui->variationlabel6->text().toInt(&ok,10)-1);
        }
        break;

    case TIME_GROUP(7,FORWARD):
        if(ui->dutylabel7->text().toInt(&ok)<600)
        {
            ui->dutylabel7->setNum(ui->dutylabel7->text().toInt(&ok,10)+1);
            ui->variationlabel7->setNum(ui->variationlabel7->text().toInt(&ok,10)+1);
        }
        break;
    case TIME_GROUP(7,BACK):
        if(ui->dutylabel7->text().toInt(&ok)>0)
        {
            ui->dutylabel7->setNum(ui->dutylabel7->text().toInt(&ok,10)-1);
            ui->variationlabel7->setNum(ui->variationlabel7->text().toInt(&ok,10)-1);
        }
        break;

    case TIME_GROUP(8,FORWARD):
        if(ui->dutylabel8->text().toInt(&ok)<600)
        {
            ui->dutylabel8->setNum(ui->dutylabel8->text().toInt(&ok,10)+1);
            ui->variationlabel8->setNum(ui->variationlabel8->text().toInt(&ok,10)+1);
        }
        break;
    case TIME_GROUP(8,BACK):
        if(ui->dutylabel8->text().toInt(&ok)>0)
        {
            ui->dutylabel8->setNum(ui->dutylabel8->text().toInt(&ok,10)-1);
            ui->variationlabel8->setNum(ui->variationlabel8->text().toInt(&ok,10)-1);
        }
        break;

    case TIME_GROUP(9,FORWARD):
        if(ui->dutylabel9->text().toInt(&ok)<600)
        {
            ui->dutylabel9->setNum(ui->dutylabel9->text().toInt(&ok,10)+1);
            ui->variationlabel9->setNum(ui->variationlabel9->text().toInt(&ok,10)+1);
        }
        break;
    case TIME_GROUP(9,BACK):
        if(ui->dutylabel9->text().toInt(&ok)>0)
        {
            ui->dutylabel9->setNum(ui->dutylabel9->text().toInt(&ok,10)-1);
            ui->variationlabel9->setNum(ui->variationlabel9->text().toInt(&ok,10)-1);
        }
        break;

    case TIME_GROUP(10,FORWARD):
        if(ui->dutylabel10->text().toInt(&ok)<600)
        {
            ui->dutylabel10->setNum(ui->dutylabel10->text().toInt(&ok,10)+1);
            ui->variationlabel10->setNum(ui->variationlabel10->text().toInt(&ok,10)+1);
        }
        break;
    case TIME_GROUP(10,BACK):
        if(ui->dutylabel10->text().toInt(&ok)>0)
        {
            ui->dutylabel10->setNum(ui->dutylabel10->text().toInt(&ok,10)-1);
            ui->variationlabel10->setNum(ui->variationlabel10->text().toInt(&ok,10)-1);
        }
        break;
    }
    int temp;
    if(group_find_value<0)
    {
        temp=-group_find_value;
    }
    else
    {
        temp=group_find_value;
    }
    usart_send(temp-1,0x50);
}

void MainWindow::timer2_close()
{
    if(timer2IsRun)
    {
        this->killTimer(timerID2);
        timer2IsRun=false;
    }
}


void MainWindow::on_forwardButton1_clicked()
{
    timer2_close();
}

void MainWindow::on_forwardButton2_clicked()
{
    timer2_close();
}

void MainWindow::on_forwardButton3_clicked()
{
    timer2_close();
}

void MainWindow::on_forwardButton4_clicked()
{
    timer2_close();
}

void MainWindow::on_forwardButton5_clicked()
{
    timer2_close();
}

void MainWindow::on_forwardButton6_clicked()
{
    timer2_close();
}

void MainWindow::on_forwardButton7_clicked()
{
    timer2_close();
}

void MainWindow::on_forwardButton8_clicked()
{
    timer2_close();
}

void MainWindow::on_forwardButton9_clicked()
{
    timer2_close();
}

void MainWindow::on_forwardButton10_clicked()
{
    timer2_close();
}



void MainWindow::on_reverseButton1_clicked()
{
    timer2_close();
}

void MainWindow::on_reverseButton2_clicked()
{
    timer2_close();
}

void MainWindow::on_reverseButton3_clicked()
{
    timer2_close();
}

void MainWindow::on_reverseButton4_clicked()
{
    timer2_close();
}

void MainWindow::on_reverseButton5_clicked()
{
    timer2_close();
}

void MainWindow::on_reverseButton6_clicked()
{
    timer2_close();
}

void MainWindow::on_reverseButton7_clicked()
{
    timer2_close();
}

void MainWindow::on_reverseButton8_clicked()
{
    timer2_close();
}

void MainWindow::on_reverseButton9_clicked()
{
    timer2_close();
}

void MainWindow::on_reverseButton10_clicked()
{
    timer2_close();
}

void MainWindow::timer2_open()
{
    if(timer2IsRun==false)
    {
        timerID2=this->startTimer(100);
        timer2IsRun=true;
    }
}

void MainWindow::buttonpressed_deal(int index, int dir)
{
    bool ok;
    if(serial.isOpen() == false)
    {
        QMessageBox::warning(NULL,"Warning","请打开串口！");
        return;
    }

    if(dir==FORWARD && dutylabel[index]->text().toInt(&ok)<600)
    {
        dutylabel[index]->setNum(dutylabel[index]->text().toInt(&ok,10)+1);
        varlabel[index]->setNum(varlabel[index]->text().toInt(&ok,10)+1);

    }
    else if(dir==BACK && dutylabel[index]->text().toInt(&ok)>0)
    {
        dutylabel[index]->setNum(dutylabel[index]->text().toInt(&ok,10)-1);
        varlabel[index]->setNum(varlabel[index]->text().toInt(&ok,10)-1);
    }

    timer2_open();
    delay=3;
    group_find_value=TIME_GROUP((index+1),dir);
    usart_send(index,0x50);
}

void MainWindow::on_forwardButton1_pressed()
{
    buttonpressed_deal(0,FORWARD);
}

void MainWindow::on_forwardButton2_pressed()
{
    buttonpressed_deal(1,FORWARD);
}

void MainWindow::on_forwardButton3_pressed()
{
    buttonpressed_deal(2,FORWARD);
}

void MainWindow::on_forwardButton4_pressed()
{
    buttonpressed_deal(3,FORWARD);
}

void MainWindow::on_forwardButton5_pressed()
{
    buttonpressed_deal(4,FORWARD);
}

void MainWindow::on_forwardButton6_pressed()
{
    buttonpressed_deal(5,FORWARD);
}

void MainWindow::on_forwardButton7_pressed()
{
    buttonpressed_deal(6,FORWARD);
}

void MainWindow::on_forwardButton8_pressed()
{
    buttonpressed_deal(7,FORWARD);
}

void MainWindow::on_forwardButton9_pressed()
{
    buttonpressed_deal(8,FORWARD);
}

void MainWindow::on_forwardButton10_pressed()
{
    buttonpressed_deal(9,FORWARD);
}



void MainWindow::on_reverseButton1_pressed()
{
    buttonpressed_deal(0,BACK);
}

void MainWindow::on_reverseButton2_pressed()
{
    buttonpressed_deal(1,BACK);
}

void MainWindow::on_reverseButton3_pressed()
{
    buttonpressed_deal(2,BACK);
}

void MainWindow::on_reverseButton4_pressed()
{
    buttonpressed_deal(3,BACK);
}

void MainWindow::on_reverseButton5_pressed()
{
    buttonpressed_deal(4,BACK);
}

void MainWindow::on_reverseButton6_pressed()
{
    buttonpressed_deal(5,BACK);
}

void MainWindow::on_reverseButton7_pressed()
{
    buttonpressed_deal(6,BACK);
}

void MainWindow::on_reverseButton8_pressed()
{
    buttonpressed_deal(7,BACK);
}

void MainWindow::on_reverseButton9_pressed()
{
    buttonpressed_deal(8,BACK);
}

void MainWindow::on_reverseButton10_pressed()
{
    buttonpressed_deal(9,BACK);
}
