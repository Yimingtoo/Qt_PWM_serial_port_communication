#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QByteArray>

namespace Ui {
class Setting;
}

class Setting : public QWidget
{
    Q_OBJECT

public:

    explicit Setting(QWidget *parent = nullptr);

    ~Setting();

    void Setting_init();

    void settabIndex(int i);

    void lineEdit_clear();

    void getSettingStatus();


signals:

    void pinsetting(QString *strs);

    void wheel_open(bool isok);

    void PID_open(bool isok);

    void addbandrate(QString bandrate);

private slots:

    void on_okButton_clicked();

    void on_addButton_clicked();

private:

    Ui::Setting *ui;

    bool wheel_open_flag;
    bool PID_open_flag;

};

#endif // SETTING_H
