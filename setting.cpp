#include "setting.h"
#include "ui_setting.h"

Setting::Setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);

    Setting_init();

    this->setWindowTitle("设置");

    this->setWindowIcon(QIcon("./icon.ico"));

    connect(ui->cancelButton,&QPushButton::clicked,this,[=]()
    {
        close();
    });

    connect(this,&Setting::destroyed,[=]()
    {
        qDebug()<<"destroy";
    });
}

Setting::~Setting()
{
    delete ui;
}

void Setting::Setting_init()
{

    int index[8];

    QFile file("PinSetting.dat");
    bool res=file.open(QIODevice::ReadWrite);
    if(false==res)
    {
        qDebug()<<"打开PinSetting.dat失败\n";
        return;
    }

    QString pin_str=file.readAll();
    index[0] = ui->comboBox_1->findText(pin_str.left(3));
    pin_str=pin_str.remove(0,4);
    index[1] = ui->comboBox_2->findText(pin_str.left(3));
    pin_str=pin_str.remove(0,4);
    index[2] = ui->comboBox_3->findText(pin_str.left(3));
    pin_str=pin_str.remove(0,4);
    index[3] = ui->comboBox_4->findText(pin_str.left(3));
    pin_str=pin_str.remove(0,4);
    index[4] = ui->comboBox_5->findText(pin_str.left(3));
    pin_str=pin_str.remove(0,4);
    index[5] = ui->comboBox_6->findText(pin_str.left(3));
    pin_str=pin_str.remove(0,4);
    index[6] = ui->comboBox_7->findText(pin_str.left(3));
    pin_str=pin_str.remove(0,4);
    index[7] = ui->comboBox_8->findText(pin_str.left(3));
    pin_str=pin_str.remove(0,4);

    ui->comboBox_1->setCurrentIndex(index[0]);
    ui->comboBox_2->setCurrentIndex(index[1]);
    ui->comboBox_3->setCurrentIndex(index[2]);
    ui->comboBox_4->setCurrentIndex(index[3]);
    ui->comboBox_5->setCurrentIndex(index[4]);
    ui->comboBox_6->setCurrentIndex(index[5]);
    ui->comboBox_7->setCurrentIndex(index[6]);
    ui->comboBox_8->setCurrentIndex(index[7]);

    file.close();

    QFile file1("settings.dat");
    res=file1.open(QIODevice::ReadOnly);
    if(false==res)
    {
        qDebug()<<"settings.dat失败\n";
        return;
    }

    QString str=file1.readAll();

    while(str!="\0")
    {
        QString property,value;

        if(str.left(1)=="<")
        {
            str.remove(0,1);
            while(str.left(1)!=" ")
            {
                property=property+str.left(1);
                str.remove(0,1);
            }
            str.remove(0,1);
            while(str.left(2)!="/>")
            {
                value=value+str.left(1);
                str.remove(0,1);
            }
            if(property=="wheel_open")
            {
                if(value=="true")
                {
                    ui->wheelBox->setCheckState(Qt::Checked);
                    wheel_open_flag=true;
                }
                else if(value=="false")
                {
                    ui->wheelBox->setCheckState(Qt::Unchecked);
                    wheel_open_flag=false;
                }
            }
            if(property=="PID_open")
            {
                if(value=="true")
                {
                    ui->isPIDopenBox->setCheckState(Qt::Checked);
                    PID_open_flag=true;
                }
                else if(value=="false")
                {
                    ui->isPIDopenBox->setCheckState(Qt::Unchecked);
                    PID_open_flag=false;
                }
            }
        }
        else
        {
            str.remove(0,1);
        }
    }
    file1.close();

}

void Setting::on_okButton_clicked()
{
    {
        QFile file("settings.dat");
        bool res=file.open(QIODevice::WriteOnly);
        if(false==res)
        {
            qDebug()<<"settings.dat失败\n";
            return;
        }
        QByteArray write_text;
        if(ui->wheelBox->isChecked())
        {
            bool ok=true;
            emit wheel_open(ok);
            write_text="<wheel_open true/>\n";
        }
        else
        {
            bool ok=false;
            emit wheel_open(ok);
            write_text="<wheel_open false/>\n";
        }

        if(ui->isPIDopenBox->isChecked())
        {
            emit PID_open(true);
            write_text+="<PID_open true/>";
        }
        else
        {
            emit PID_open(false);
            write_text+="<PID_open false/>";
        }
        file.write(write_text);

        file.close();
    }

    {
        int index[8];
        QString title[8];
        index[0]=ui->comboBox_1->currentIndex();
        index[1]=ui->comboBox_2->currentIndex();
        index[2]=ui->comboBox_3->currentIndex();
        index[3]=ui->comboBox_4->currentIndex();
        index[4]=ui->comboBox_5->currentIndex();
        index[5]=ui->comboBox_6->currentIndex();
        index[6]=ui->comboBox_7->currentIndex();
        index[7]=ui->comboBox_8->currentIndex();
        for(int i=0;i<8;i++)
        {
            for(int j=i+1;j<8;j++)
            {
                if(index[i]==index[j])
                {
                    QMessageBox::warning(NULL,"警告","各组引脚号不能重复！");
                    return;
                }
            }
        }
        QFile filesaves("PinSetting.dat");
        bool res=filesaves.open(QIODevice::ReadWrite);
        if(false==res)
        {
            qDebug()<<"打开IARpath.txt失败\n";
            return;
        }
        for(int i=0;i<8;i++)
        {
            title[i]=ui->comboBox_1->itemText(index[i]);
            filesaves.write(title[i].toUtf8()+"\n");
        }
        filesaves.close();

        emit pinsetting(title);
    }

    if(ui->tabWidget->currentIndex()==2)
    {
        QString bandrate;
        bool ok;
        bandrate=ui->lineEdit->text();
        if(bandrate=="")
        {
            return;
        }
        if(0==bandrate.toInt(&ok))
        {
            QMessageBox::warning(NULL,"警告","\n有非数字字符");
            ui->lineEdit->clear();
            return;
        }
        ui->lineEdit->clear();
        emit addbandrate(bandrate);
        if(QMessageBox::Ok==QMessageBox::information(NULL,"提示","波特率添加成功！"))
        {
            this->close();
        }
    }

    close();
}

void Setting::on_addButton_clicked()
{
    QString bandrate;
    bool ok;
    bandrate=ui->lineEdit->text();
    if(bandrate=="")
    {
        return;
    }
    if(0==bandrate.toInt(&ok))
    {
        QMessageBox::warning(NULL,"警告","\n有非数字字符");
        ui->lineEdit->clear();
        return;
    }
    ui->lineEdit->clear();
    emit addbandrate(bandrate);
    if(QMessageBox::Ok==QMessageBox::information(NULL,"提示","波特率添加成功！"))
    {
        this->close();
    }

}

void Setting::settabIndex(int i)
{
    ui->tabWidget->setCurrentIndex(i);
}

void Setting::lineEdit_clear()
{
    ui->lineEdit->clear();
}

void Setting::getSettingStatus()
{
    if(wheel_open_flag)
    {
        emit wheel_open(true);
    }
    else
    {
        emit wheel_open(false);
    }

    if(PID_open_flag)
    {
        emit PID_open(true);
    }
    else
    {
        emit PID_open(false);
    }
}

