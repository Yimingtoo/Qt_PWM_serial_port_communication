#include "mainwindow.h"
#include "ui_mainwindow.h"

//------------------------------------------------------------------------------------------------>自定义函数

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
    download_flag=false;
    part_download_flag=false;
    repeat_flag=false;
    is_usr_change_flag=false;
    copy_flag=false;
    //代码表格

    ui->codetableWidget->setColumnCount(2);//设置列数

    ui->codetableWidget->setHorizontalHeaderLabels(QStringList()<<"引脚号"<<"目标量");//设置水平表头
    ui->codetableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->codetableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->codetableWidget->setColumnWidth(1,40);
    ui->codetableWidget->setColumnWidth(0,70);
    ui->codetableWidget->setColumnWidth(2,70);
    ui->codetableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->codetableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::AnyKeyPressed);

    //右击菜单

    ui->codetableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    table_widget_menu = new QMenu(ui->codetableWidget);

    QAction *action = new QAction("添加(Insert)", this);
    connect(action,&QAction::triggered, this,&MainWindow::slotActionInsert);
    table_widget_menu->addAction(action);

    action = new QAction("删除(Delete)", this);
    connect(action,&QAction::triggered, this,&MainWindow::slotActionDelete);
    table_widget_menu->addAction(action);

    action = new QAction("复制(Ctrl+C)", this);
    connect(action,&QAction::triggered, this,&MainWindow::slotActionCopy);
    table_widget_menu->addAction(action);

    action = new QAction("粘贴(Ctrl+V)", this);
    connect(action,&QAction::triggered, this,&MainWindow::slotActionPaste);
    table_widget_menu->addAction(action);


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

void MainWindow::download_data()
{
    //qDebug()<<code_count;
    bool ok;
    int i;

    unsigned char temphigh,templow;
    unsigned char pin_id;

    if(code_count>=0)
    {
        for(i=0;i<10;i++)
        {
            if(group[i]->title()==pinlist[code_count])
                break;
        }

        pin_id=find_pinID(pinlist[code_count]);
        if(pin_id==0xff)
            return;
        temphigh=(unsigned char)(aimlist[code_count].toInt(&ok)>>8);
        templow=(unsigned char)(aimlist[code_count].toInt(&ok));
    }
    else
    {
        temphigh=0xfc;
        templow=0;
    }
    if(download_flag==true)
    {
        if(code_count<pinlist.size()-1)
        {
            char datahigh=(char)(pin_id|temphigh);
            char datalow=(char)templow;
            serial.write(&datahigh,1);
            serial.write(&datalow,1);
        }
        else
        {
            char datahigh=(char)0x73;
            char datalow=(char)0x74;
            serial.write(&datahigh,1);
            serial.write(&datalow,1);
            code_count=0;
            download_flag=false;
            ui->downloadButton->setText("下载");

        }
    }
    else if(part_download_flag==true)
    {
        if(code_count<=max_row && code_count<pinlist.size()-1)
        {
            char datahigh=(char)(pin_id|temphigh);
            char datalow=(char)templow;
            serial.write(&datahigh,1);
            serial.write(&datalow,1);
        }
        else
        {
            char datahigh=(char)0x73;
            char datalow=(char)0x74;
            serial.write(&datahigh,1);
            serial.write(&datalow,1);
            code_count=0;
            part_download_flag=false;
            ui->part_down_Button->setText("局部下载");
        }
    }
    else if(repeat_flag==true)
    {

        if(code_count==min_row-1)
        {
            char cmd=0x60;
            serial.write(&cmd,1);
        }
        else
        {
            if(code_count<=max_row && code_count<pinlist.size()-1)
            {
                char datahigh=(char)(pin_id|temphigh);
                char datalow=(char)templow;
                serial.write(&datahigh,1);
                serial.write(&datalow,1);
            }
            else
            {
                char datahigh=(char)0x73;
                char datalow=(char)0x74;
                serial.write(&datahigh,1);
                serial.write(&datalow,1);
                code_count=min_row-2;
                ui->part_down_Button->setText("局部下载");
            }
        }
    }
    code_count++;
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

void MainWindow::timer2_open()
{
    if(timer2IsRun==false)
    {
        timerID2=this->startTimer(50);
        timer2IsRun=true;
    }
}

void MainWindow::timer2_close()
{
    if(timer2IsRun)
    {
        this->killTimer(timerID2);
        timer2IsRun=false;
    }
}

void MainWindow::usart_send(int i, char cmd)
{
    bool ok;
    unsigned char pin_id;
    char senddat[4];
    senddat[0]=cmd;
    pin_id=find_pinID(group[i]->title());

    QString dutynum=dutylabel[i]->text();
    unsigned int duty_int=dutynum.toInt(&ok);

    //qDebug()<<pin_id;

    unsigned char tempHigh=(unsigned char)(duty_int>>8);
    unsigned char tempLow=(unsigned char)(duty_int);

    senddat[1]=pin_id|tempHigh;
    senddat[2]=tempLow;

    senddat[3]='\0';
    QByteArray byte=senddat;
    //qDebug()<<byte;
    serial.write(senddat,3);
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

void MainWindow::write_debug(QString path)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    for(int i=0;i<pinlist.size();i++)
    {
        if(pinlist[i]=="")
        {
            file.write("000 "+aimlist[i].toUtf8()+"\n");
        }
        else
        {
            file.write(pinlist[i].toUtf8()+" "+aimlist[i].toUtf8()+"\n");
        }

    }
    file.close();
}

void MainWindow::copy_table()
{
    QList<QTableWidgetItem*>items=ui->codetableWidget->selectedItems();
    int temp_count=items.count();
    min_row=ui->codetableWidget->row(items.at(0));
    max_row=ui->codetableWidget->row(items.at(temp_count-1));

    for(int i=min_row;i<=max_row;i++)
    {
        copypin<<pinlist[i];
        copyaim<<aimlist[i];
    }
    qDebug()<<copypin<<copyaim;
}

void MainWindow::paste_table()
{
    int copysize=copypin.size();

    for(int i=copysize-1;i>=0;i--)
    {
        pinlist.insert(pressed_row,copypin[i]);
        aimlist.insert(pressed_row,copyaim[i]);
        qDebug()<<pinlist<<aimlist;
    }
    int temp_count=pinlist.size();
    ui->codetableWidget->setRowCount(temp_count);
    for(int i=0;i<temp_count;i++)
    {
        int col=0;
        ui->codetableWidget->setItem(i,col++,new QTableWidgetItem(pinlist[i]));
        ui->codetableWidget->setItem(i,col++,new QTableWidgetItem(aimlist[i]));

    }
}

//------------------------------------------------------------------------------------------------>公有槽函数

void MainWindow::saveas_action_res()
{
    bool ok;
    QString file_path=QFileDialog::getOpenFileName(this,"工程文件路径","../","prj(*.prj);;All(*.*)");
    if(file_path=="")
    {
        return;
    }

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
    file.open(QIODevice::Append);

    file.write(QByteArray("#ifndef __CTRL_H\n"));
    file.write(QByteArray("#define __CTRL_H\n"));
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

    {
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
        QFile file(prjpath+"Reset_value.h");

        QFile file2("PinSetting.dat");
        bool res=file2.open(QIODevice::ReadWrite);
        if(false==res)
        {
            qDebug()<<"打开PinSetting.dat失败\n";
            return;
        }

        QString titlename[10];

        QString pin_str=file2.readAll();
        for(int i=0;i<10;i++)
        {
            titlename[i]=pin_str.left(3);
            pin_str=pin_str.remove(0,4);
        }

        file2.close();

        QFile file3("resetvalue.dat");
        res=file3.open(QIODevice::ReadOnly);
        if(false==res)
        {
            qDebug()<<"打开resetvalue.dat失败\n";
            return;
        }
        QString resetvalue=file3.readAll();

        file3.close();

        file.open(QIODevice::WriteOnly);
        file.write(QByteArray("#ifndef __RESET_VALUE_H\n"));
        file.write(QByteArray("#define __RESET_VALUE_H\n\n"));
        file.write(QByteArray("unsigned int reset_value[10] =\n"));
        file.write(QByteArray("{\n"));
        for(int i=0;i<10;i++)
        {
            file.write("    " + file_search(titlename[i],resetvalue).toUtf8());
            if(i!=9)
                file.write("," + QString("    //%1").arg(group[i]->title()).toUtf8() + "\n");
        }

        file.write(QByteArray("\n};\n\n"));
        file.write(QByteArray("#endif\n"));
        file.close();
    }
}

void MainWindow::writereset_action_res()
{
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
    QFile file(prjpath+"Reset_value.h");

    QFile file2("PinSetting.dat");
    bool res=file2.open(QIODevice::ReadWrite);
    if(false==res)
    {
        qDebug()<<"打开PinSetting.dat失败\n";
        return;
    }

    QString titlename[10];

    QString pin_str=file2.readAll();
    for(int i=0;i<10;i++)
    {
        titlename[i]=pin_str.left(3);
        pin_str=pin_str.remove(0,4);
    }
    //qDebug()<<titlename;

    file2.close();

    QFile file3("resetvalue.dat");
    res=file3.open(QIODevice::ReadOnly);
    if(false==res)
    {
        qDebug()<<"打开resetvalue.dat失败\n";
        return;
    }
    QString resetvalue=file3.readAll();

    file3.close();

    file.open(QIODevice::WriteOnly);
    file.write(QByteArray("#ifndef __RESET_VALUE_H\n"));
    file.write(QByteArray("#define __RESET_VALUE_H\n\n"));
    file.write(QByteArray("unsigned int reset_value[10] =\n"));
    file.write(QByteArray("{\n"));
    for(int i=0;i<10;i++)
    {
        file.write("    " + file_search(titlename[i],resetvalue).toUtf8());
        if(i!=9)
            file.write("," + QString("    //%1").arg(group[i]->title()).toUtf8() + "\n");
    }

    file.write(QByteArray("\n};\n\n"));
    file.write(QByteArray("#endif\n"));
    file.close();
}

void MainWindow::slotActionInsert()
{
    is_usr_change_flag=true;
    pinlist.insert(pressed_row,"");
    aimlist.insert(pressed_row,"");

    ui->codetableWidget->setRowCount(pinlist.size());

    for(int i=0;i<pinlist.size();i++)
    {
        int col=0;
        ui->codetableWidget->setItem(i,col++,new QTableWidgetItem(pinlist[i]));
        ui->codetableWidget->setItem(i,col++,new QTableWidgetItem(aimlist[i]));
    }
    ui->codetableWidget->scrollToBottom();  //让滚动条滚动到最底
    is_usr_change_flag=false;
}

void MainWindow::slotActionDelete()
{
    is_usr_change_flag=true;
    pinlist.removeAt(pressed_row);
    aimlist.removeAt(pressed_row);

    ui->codetableWidget->setRowCount(pinlist.size());

    for(int i=0;i<pinlist.size();i++)
    {
        int col=0;
        ui->codetableWidget->setItem(i,col++,new QTableWidgetItem(pinlist[i]));
        ui->codetableWidget->setItem(i,col++,new QTableWidgetItem(aimlist[i]));

    }
    ui->codetableWidget->scrollToBottom();  //让滚动条滚动到最底
    is_usr_change_flag=false;
}

void MainWindow::slotActionCopy()
{
    copy_table();
    copy_flag=true;
}

void MainWindow::slotActionPaste()
{
    if(copy_flag)
    {
        paste_table();
    }
}

void MainWindow::debug_save_action_rec()
{
    write_debug("debug_save.dat");
}

void MainWindow::debug_save_as_action_rec()
{
    QString file_name=QFileDialog::getSaveFileName(this,"调试另存为","../",".dat(*.dat);;All(*.*)");
    if(file_name=="")
    {
        return;
    }
    write_debug(file_name);
}
void MainWindow::read_debug(QString path)
{
    QFile file(path);
    QFileInfo fi(file);
    file.open(QIODevice::ReadOnly);
    QString data_str=file.readAll();
    file.close();

    int count=0;
    while(data_str!="")
    {
        if(data_str.left(3)=="000")
        {
            pinlist.insert(count,"");
            data_str.remove(0,4);
            QString temp="";
            while(data_str.left(1)!="\n")
            {
                temp=temp+data_str.left(1);
                data_str.remove(0,1);
            }
            aimlist.insert(count,temp);
            data_str.remove(0,1);

        }
        else if(data_str.left(3)=="Spe" || data_str.left(3)=="Del")
        {
            pinlist.insert(count,data_str.left(5));
            data_str.remove(0,6);
            QString temp="";
            while(data_str.left(1)!="\n")
            {
                temp=temp+data_str.left(1);
                data_str.remove(0,1);
            }
            aimlist.insert(count,temp);
            data_str.remove(0,1);
        }
        else
        {
            pinlist.insert(count,data_str.left(3));
            data_str.remove(0,4);
            QString temp="";
            while(data_str.left(1)!="\n")
            {
                temp=temp+data_str.left(1);
                data_str.remove(0,1);

            }
            aimlist.insert(count,temp);
            data_str.remove(0,1);

        }
        count++;
    }
    pinlist.replace(count-1,"");
    if(fi.fileName()=="debug_save.dat")
    {
        aimlist.replace(count-1,QString("^系统保存调试^"));
    }
    else
    {
        aimlist.replace(count-1,QString("^%1^").arg(fi.fileName()));
    }

    ui->codetableWidget->setRowCount(pinlist.size());

    for(int i=0;i<pinlist.size();i++)
    {
        qDebug()<<i;
        int col=0;
        ui->codetableWidget->setItem(i,col++,new QTableWidgetItem(pinlist[i]));
        ui->codetableWidget->setItem(i,col++,new QTableWidgetItem(aimlist[i]));

    }
    ui->codetableWidget->scrollToBottom();  //让滚动条滚动到最底
}
void MainWindow::open_action_rec()
{
    read_debug("debug_save.dat");
}

void MainWindow::open_from_action_rec()
{
    QString file_name=QFileDialog::getOpenFileName(this,"调试路径","../","All(*.*)");
    if(file_name=="")
    {
        return;
    }
    read_debug(file_name);
}

void MainWindow::pinsettingrec(QString *title)
{
    for(int i=0;i<10;i++)
    {
        group[i]->setTitle(title[i]);
    }

    QString write="";
    QFile file("resetvalue.dat");
    bool res=file.open(QIODevice::WriteOnly);
    if(false==res)
    {
        qDebug()<<"打开resetvalue.dat失败\n";
        return;
    }
    for(int i=0;i<10;i++)
        write=write+title[i]+" "+QString::number(300)+"\n";
    file.write(write.toUtf8());
    file.close();

}

//------------------------------------------------------------------------------------------------>私有槽函数

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

void MainWindow::on_clearButton_clicked()
{
    ui->codetableWidget->setRowCount(0);
    pinlist.clear();
    varlist.clear();
    aimlist.clear();
}

void MainWindow::on_generateButton_clicked()
{
    is_usr_change_flag=true;
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

    for(int i=current_row;i<current_row+count;i++)
    {
        int col=0;
        ui->codetableWidget->setItem(i,col++,new QTableWidgetItem(pinlist[i]));
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
    is_usr_change_flag=false;
}

void MainWindow::on_downloadButton_clicked()
{

    if(ui->open_Button->text()=="打开串口")
    {
        QMessageBox::warning(NULL,"警告","请打开串口");
        return;
    }
    char cmd=0x60;
    if(download_flag==false && part_download_flag==false && repeat_flag==false)
    {
        code_count=0;
        download_flag=true;
        serial.write(&cmd,1);
        ui->downloadButton->setText("停止");
    }
    else if(download_flag==true)
    {
        code_count=0;
        download_flag=false;
        ui->downloadButton->setText("下载");
    }
}

void MainWindow::on_part_down_Button_clicked()
{
    if(ui->open_Button->text()=="打开串口")
    {
        QMessageBox::warning(NULL,"警告","请打开串口");
        return;
    }
    if(download_flag==false && part_download_flag==false && repeat_flag==false)
    {
        part_download_flag=true;

        QList<QTableWidgetItem*>items=ui->codetableWidget->selectedItems();
        int temp_count=items.count();
        min_row=ui->codetableWidget->row(items.at(0));
        max_row=ui->codetableWidget->row(items.at(temp_count-1));
        if(min_row==-1 || max_row==-1)
        {
            return;
        }

        qDebug()<<min_row;
        code_count=min_row;
        char cmd=0x60;
        serial.write(&cmd,1);
        ui->part_down_Button->setText("停止");
    }
    else if(part_download_flag==true)
    {
        code_count=0;
        part_download_flag=false;
        ui->part_down_Button->setText("局部下载");
    }

}

void MainWindow::on_repeat_download_Button_clicked()
{
    if(ui->open_Button->text()=="打开串口")
    {
        QMessageBox::warning(NULL,"警告","请打开串口");
        return;
    }
    if(download_flag==false && part_download_flag==false && repeat_flag==false)
    {
        repeat_flag=true;

        QList<QTableWidgetItem*>items=ui->codetableWidget->selectedItems();
        int temp_count=items.count();
        min_row=ui->codetableWidget->row(items.at(0));
        max_row=ui->codetableWidget->row(items.at(temp_count-1));
        if(min_row==-1 || max_row==-1)
        {
            return;
        }

        qDebug()<<min_row;
        code_count=min_row;
        char cmd=0x60;
        serial.write(&cmd,1);
        ui->repeat_download_Button->setText("停止");
    }
    else if(repeat_flag==true)
    {
        code_count=0;
        repeat_flag=false;
        ui->repeat_download_Button->setText("重复下载");
    }

}

void MainWindow::on_codetableWidget_cellChanged(int row, int column)
{
    bool ok;
    if(is_usr_change_flag)
    {
        return;
    }
    else
    {
        if(column==0)
        {
            QString getdata=ui->codetableWidget->item(row,column)->data(Qt::DisplayRole).toString();
            if( getdata=="PA0"  ||
                getdata=="PA1"  ||
                getdata=="PA2"  ||
                getdata=="PA3"  ||
                getdata=="PA4"  ||
                getdata=="PA5"  ||
                getdata=="PA6"  ||
                getdata=="PA7"  ||

                getdata=="PB0"  ||
                getdata=="PB1"  ||
                getdata=="PB2"  ||
                getdata=="PB3"  ||
                getdata=="PB4"  ||
                getdata=="PB5"  ||
                getdata=="PB6"  ||
                getdata=="PB7"  ||

                getdata=="PC0"  ||
                getdata=="PC1"  ||
                getdata=="PC2"  ||
                getdata=="PC3"  ||
                getdata=="PC4"  ||
                getdata=="PC5"  ||
                getdata=="PC6"  ||
                getdata=="PC7"  ||

                getdata=="PD0"  ||
                getdata=="PD1"  ||
                getdata=="PD2"  ||
                getdata=="PD3"  ||
                getdata=="PD4"  ||
                getdata=="PD5"  ||
                getdata=="PD6"  ||
                getdata=="PD7"  ||
                getdata==""     ||
                getdata=="Speed"||
                getdata=="Delay")
            {
                pinlist.replace(row,getdata);
            }
            else
            {
                ui->codetableWidget->setItem(row,column,new QTableWidgetItem(tabledata));
            }

        }
        if(column==1)
        {
            QString getdata=ui->codetableWidget->item(row,column)->data(Qt::DisplayRole).toString();
            QString col0=pinlist[row];
            qDebug()<<col0;
            if( col0=="PA0"  ||
                col0=="PA1"  ||
                col0=="PA2"  ||
                col0=="PA3"  ||
                col0=="PA4"  ||
                col0=="PA5"  ||
                col0=="PA6"  ||
                col0=="PA7"  ||

                col0=="PB0"  ||
                col0=="PB1"  ||
                col0=="PB2"  ||
                col0=="PB3"  ||
                col0=="PB4"  ||
                col0=="PB5"  ||
                col0=="PB6"  ||
                col0=="PB7"  ||

                col0=="PC0"  ||
                col0=="PC1"  ||
                col0=="PC2"  ||
                col0=="PC3"  ||
                col0=="PC4"  ||
                col0=="PC5"  ||
                col0=="PC6"  ||
                col0=="PC7"  ||

                col0=="PD0"  ||
                col0=="PD1"  ||
                col0=="PD2"  ||
                col0=="PD3"  ||
                col0=="PD4"  ||
                col0=="PD5"  ||
                col0=="PD6"  ||
                col0=="PD7")
            {
                if((getdata.toInt(&ok)<=600 && getdata.toInt(&ok)>0) | getdata=="0")
                {
                    aimlist.replace(row,getdata);
                }
                else
                {
                    ui->codetableWidget->setItem(row,column,new QTableWidgetItem(tabledata));
                }
            }
            else if(col0=="Speed")
            {
                if(getdata.toInt(&ok)<=50 && getdata.toInt(&ok)>0)
                {
                    aimlist.replace(row,getdata);
                }
                else
                {
                    ui->codetableWidget->setItem(row,column,new QTableWidgetItem(tabledata));
                }
            }
            else if(col0=="Delay")
            {
                if((getdata.toInt(&ok)<=10000 && getdata.toInt(&ok)>0))
                {
                    aimlist.replace(row,getdata);
                }
                else
                {
                    ui->codetableWidget->setItem(row,column,new QTableWidgetItem(tabledata));
                }
            }
            else if(col0=="")
            {
                aimlist.replace(row,getdata);
            }
        }
    }
}

void MainWindow::on_codetableWidget_cellPressed(int row, int column)
{
    tabledata = ui->codetableWidget->item(row,column)->data(Qt::DisplayRole).toString();
    pressed_row=row;
    qDebug()<<pressed_row<<tabledata;
}

void MainWindow::on_codetableWidget_cellDoubleClicked(int row, int column)
{
    button= qApp->mouseButtons();
    if(button==Qt::RightButton)
    {
        ui->codetableWidget->removeRow(row);
        pinlist.removeAt(row);
        varlist.removeAt(row);
        aimlist.removeAt(row);
    }
}

void MainWindow::on_codetableWidget_customContextMenuRequested(const QPoint &pos)
{
    table_widget_menu->exec(QCursor::pos());
}

//======================================>正向开始

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

//======================================>反向开始

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

//======================================>正向结束

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

//======================================>反向结束

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


