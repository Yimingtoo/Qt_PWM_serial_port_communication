#include "mainwindow.h"
#include "ui_mainwindow.h"

//------------------------------------------------------------------------------------------------>自定义函数

void MainWindow::bandrate_init()
{
    ui->bandRateBox->clear();
    QFile file("bandrate.dat");
    bool res=file.open(QIODevice::ReadOnly);
    if(res==false)
    {
        return;
    }
    QString bandrates = file.readAll();
    QString temp;
    while(bandrates!="#")
    {
        temp="";
        while(bandrates.left(1)!="\n")
        {
            temp=temp+bandrates.left(1);
            bandrates.remove(0,1);
        }

        ui->bandRateBox->addItem(temp);
        bandrates.remove(0,1);
    }
    file.close();
    ui->bandRateBox->addItem("添加");
}

void MainWindow::bandrate_delete(int index)
{
    button=Qt::NoButton;

    qDebug()<<index;
    if(ui->bandRateBox->itemText(index)=="添加")
    {
        return;
    }
    else
    {
        QMessageBox::StandardButton res=QMessageBox::warning(NULL,"警告","是否删除当前项",QMessageBox::Yes|QMessageBox::Cancel);
        if(res==QMessageBox::Yes)
        {
            ui->bandRateBox->removeItem(index);
        }
        if(ui->bandRateBox->currentText()=="添加")
        {
            ui->bandRateBox->setCurrentIndex(0);
        }
    }

    QFile file("bandrate.dat");
    bool result=file.open(QIODevice::WriteOnly);
    if(result==false)
    {
        return;
    }
    for(int i=0;i<ui->bandRateBox->count()-1;i++)
    {
        file.write(ui->bandRateBox->itemText(i).toUtf8()+"\n");
    }
    file.write("#");
    file.close();
}

//------------------------------------------------------------------------------------------------>自定义槽函数

void MainWindow::bandrate_rec(QString bandrate)
{
    bool ok;
    ui->bandRateBox->insertItem(ui->bandRateBox->count()-1,bandrate);
    QVector<int> bandrate1;
    for(int i=0;i<ui->bandRateBox->count()-1;i++)
    {
        bandrate1+=ui->bandRateBox->itemText(i).toInt(&ok);
    }
    qDebug()<<bandrate1.count()<<bandrate1;
    for(int i=0;i<ui->bandRateBox->count()-1;i++)
    {
        for(int j=0;j<ui->bandRateBox->count()-2-i;j++)
        {
            if(bandrate1.value(j)<bandrate1.value(j+1))
            {
                bandrate1.swapItemsAt(j,j+1);
            }
        }
    }
    qDebug()<<bandrate1.count()<<bandrate1;
    for(int i=0;i<ui->bandRateBox->count()-1;i++)
    {
        ui->bandRateBox->setItemText(i,QString::number(bandrate1.value(i)));
    }
    ui->bandRateBox->setCurrentIndex(ui->bandRateBox->findText(bandrate));

    QFile file("bandrate.dat");
    bool res=file.open(QIODevice::WriteOnly);
    if(res==false)
    {
        return;
    }
    for(int i=0;i<ui->bandRateBox->count()-1;i++)
    {
        file.write(ui->bandRateBox->itemText(i).toUtf8()+"\n");
    }
    file.write("#");
    file.close();
}

void MainWindow::bandrate_ishighlighted(int index)
{
    if(timer3IsRun==false)
    {
        timerID3=this->startTimer(50);
        timer3IsRun=true;
    }
    bandratebox_index=index;
}

void MainWindow::bandrateindex_ischanged(int index)
{
    if(timer3IsRun)
    {
        this->killTimer(timerID3);
        timer3IsRun=false;
    }
    if((Qt::LeftButton==button || Qt::RightButton==button) && ui->bandRateBox->itemText(index)=="添加")
    {
        button=Qt::NoButton;
        ui->bandRateBox->setCurrentIndex(0);

        setting->show();
        setting->settabIndex(2);
        setting->lineEdit_clear();
    }
    if(Qt::RightButton==button)
    {
        bandrate_delete(index);
    }
}
