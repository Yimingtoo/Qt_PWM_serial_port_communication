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

    varlabel[0]=ui->variationlabel1;
    varlabel[1]=ui->variationlabel2;
    varlabel[2]=ui->variationlabel3;
    varlabel[3]=ui->variationlabel4;
    varlabel[4]=ui->variationlabel5;
    varlabel[5]=ui->variationlabel6;
    varlabel[6]=ui->variationlabel7;
    varlabel[7]=ui->variationlabel8;

    group[0]=ui->group1;
    group[1]=ui->group2;
    group[2]=ui->group3;
    group[3]=ui->group4;
    group[4]=ui->group5;
    group[5]=ui->group6;
    group[6]=ui->group7;
    group[7]=ui->group8;

    QFile file("PinSetting.dat");
    bool res=file.open(QIODevice::ReadWrite);
    if(false==res)
    {
        qDebug()<<"打开IARpath.txt失败\n";
        return;
    }
    QString pin_str=file.readAll();
    for(int i=0;i<8;i++)
    {
        group[i]->setTitle(pin_str.left(3));
        group[i]->setStyleSheet("QGroupBox  {color: blue;}");
        pin_str=pin_str.remove(0,4);
    }

    ui->velocityBox->setMaximum(50);
    ui->delayBox->setMaximum(100);

    read_resetvalue();

    wheel_steer_flag=0;

}

void MainWindow::pinsettingrec(QString *title)
{
    for(int i=0;i<8;i++)
    {
        group[i]->setTitle(title[i]);
    }
    write_resetvalue();
}

void MainWindow::write_resetvalue()
{
    QString title[8],write;
    for(int i=0;i<8;i++)
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
    for(int i=0;i<8;i++)
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

    for(int i=0;i<8;i++)
    {
        dutylabel[i]->setText(file_search(group[i]->title(),read_str));
    }
}

void MainWindow::on_resetButton_4_clicked()
{
    read_resetvalue();

    for(int i=0;i<8;i++)
    {
        varlabel[i]->setNum(0);
    }

    ui->velocityBox->setValue(0);
    ui->delayBox->setValue(0);
}

void MainWindow::usart_send(int i)
{
    bool ok;

    QString varnum=varlabel[i]->text();
    QString dutynum=dutylabel[i]->text();
    dutynum=QString::number(dutynum.toInt(&ok),2);
    int j=10-dutynum.length();
    for(int i=0;i<j;i++)
    {
        dutynum="0"+dutynum;

    }
    QString senddatas;
    senddatas="01010000000000"+dutynum;

    int num1=senddatas.left(8).toInt(&ok,2);
    int num2=senddatas.left(16).right(8).toInt(&ok,2);
    int num3=senddatas.left(24).right(8).toInt(&ok,2);

    int *pn1=&num1;
    int *pn2=&num2;
    int *pn3=&num3;

    char*pnc1=(char*)pn1;
    char*pnc2=(char*)pn2;
    char*pnc3=(char*)pn3;

    serial.write(pnc1,1);
    serial.write(pnc2,1);
    serial.write(pnc3,1);
}

void MainWindow::group_find()
{
    bool ok;

    switch(group_find_value)
    {
    case TIME_GROUP(1,FORWARD):
        ui->dutylabel1->setNum(ui->dutylabel1->text().toInt(&ok,10)+1);
        ui->variationlabel1->setNum(ui->variationlabel1->text().toInt(&ok,10)+1);
        break;
    case TIME_GROUP(1,BACK):
        ui->dutylabel1->setNum(ui->dutylabel1->text().toInt(&ok,10)-1);
        ui->variationlabel1->setNum(ui->variationlabel1->text().toInt(&ok,10)-1);
        break;

    case TIME_GROUP(2,FORWARD):
        ui->dutylabel2->setNum(ui->dutylabel2->text().toInt(&ok,10)+1);
        ui->variationlabel2->setNum(ui->variationlabel2->text().toInt(&ok,10)+1);
        break;
    case TIME_GROUP(2,BACK):
        ui->dutylabel2->setNum(ui->dutylabel2->text().toInt(&ok,10)-1);
        ui->variationlabel2->setNum(ui->variationlabel2->text().toInt(&ok,10)-1);
        break;

    case TIME_GROUP(3,FORWARD):
        ui->dutylabel3->setNum(ui->dutylabel3->text().toInt(&ok,10)+1);
        ui->variationlabel3->setNum(ui->variationlabel3->text().toInt(&ok,10)+1);
        break;
    case TIME_GROUP(3,BACK):
        ui->dutylabel3->setNum(ui->dutylabel3->text().toInt(&ok,10)-1);
        ui->variationlabel3->setNum(ui->variationlabel3->text().toInt(&ok,10)-1);
        break;

    case TIME_GROUP(4,FORWARD):
        ui->dutylabel4->setNum(ui->dutylabel4->text().toInt(&ok,10)+1);
        ui->variationlabel4->setNum(ui->variationlabel4->text().toInt(&ok,10)+1);
        break;
    case TIME_GROUP(4,BACK):
        ui->dutylabel4->setNum(ui->dutylabel4->text().toInt(&ok,10)-1);
        ui->variationlabel4->setNum(ui->variationlabel4->text().toInt(&ok,10)-1);
        break;

    case TIME_GROUP(5,FORWARD):
        ui->dutylabel5->setNum(ui->dutylabel5->text().toInt(&ok,10)+1);
        ui->variationlabel5->setNum(ui->variationlabel5->text().toInt(&ok,10)+1);
        break;
    case TIME_GROUP(5,BACK):
        ui->dutylabel5->setNum(ui->dutylabel5->text().toInt(&ok,10)-1);
        ui->variationlabel5->setNum(ui->variationlabel5->text().toInt(&ok,10)-1);
        break;

    case TIME_GROUP(6,FORWARD):
        ui->dutylabel6->setNum(ui->dutylabel6->text().toInt(&ok,10)+1);
        ui->variationlabel6->setNum(ui->variationlabel6->text().toInt(&ok,10)+1);
        break;
    case TIME_GROUP(6,BACK):
        ui->dutylabel6->setNum(ui->dutylabel6->text().toInt(&ok,10)-1);
        ui->variationlabel6->setNum(ui->variationlabel6->text().toInt(&ok,10)-1);
        break;

    case TIME_GROUP(7,FORWARD):
        ui->dutylabel7->setNum(ui->dutylabel7->text().toInt(&ok,10)+1);
        ui->variationlabel7->setNum(ui->variationlabel7->text().toInt(&ok,10)+1);
        break;
    case TIME_GROUP(7,BACK):
        ui->dutylabel7->setNum(ui->dutylabel7->text().toInt(&ok,10)-1);
        ui->variationlabel7->setNum(ui->variationlabel7->text().toInt(&ok,10)-1);
        break;

    case TIME_GROUP(8,FORWARD):
        ui->dutylabel8->setNum(ui->dutylabel8->text().toInt(&ok,10)+1);
        ui->variationlabel8->setNum(ui->variationlabel8->text().toInt(&ok,10)+1);
        break;
    case TIME_GROUP(8,BACK):
        ui->dutylabel8->setNum(ui->dutylabel8->text().toInt(&ok,10)-1);
        ui->variationlabel8->setNum(ui->variationlabel8->text().toInt(&ok,10)-1);
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
    usart_send(temp-1);
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
    if(dir==FORWARD)
    {
        dutylabel[index]->setNum(dutylabel[index]->text().toInt(&ok,10)+1);
        varlabel[index]->setNum(varlabel[index]->text().toInt(&ok,10)+1);
    }
    else
    {
        dutylabel[index]->setNum(dutylabel[index]->text().toInt(&ok,10)-1);
        varlabel[index]->setNum(varlabel[index]->text().toInt(&ok,10)-1);
    }
    timer2_open();
    delay=3;
    group_find_value=TIME_GROUP((index+1),dir);
    usart_send(index);
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
