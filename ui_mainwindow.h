/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tebWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_6;
    QwtPlot *qwtPlot;
    QHBoxLayout *horizontalLayout;
    QFormLayout *formLayout;
    QLabel *label_2;
    QLineEdit *pEdit;
    QLabel *label_3;
    QLineEdit *iEdit;
    QLabel *label_4;
    QLineEdit *dEdit;
    QPushButton *sendPIDButton;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_4;
    QGridLayout *gridLayout_2;
    QLabel *label_8;
    QDoubleSpinBox *ymaxSpinBox;
    QLabel *label_16;
    QDoubleSpinBox *yminSpinBox;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_7;
    QLabel *label;
    QLabel *label_17;
    QGridLayout *gridLayout_3;
    QLabel *label_5;
    QLabel *peaklabel;
    QLabel *label_6;
    QLabel *valleylabel;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout;
    QPushButton *startButton;
    QPushButton *pushButton_2;
    QPushButton *fixButton;
    QLabel *warninglabel;
    QPushButton *resetButton;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_9;
    QTextEdit *ReceiveEdit;
    QLabel *label_10;
    QTextEdit *SendEdit;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QComboBox *bandRateBox;
    QLabel *label_11;
    QLabel *label_12;
    QPushButton *open_Button;
    QComboBox *stopBitBox;
    QLabel *label_13;
    QComboBox *comBox;
    QPushButton *search_Button;
    QComboBox *checkoutBox;
    QLabel *label_14;
    QComboBox *dataBitBox;
    QLabel *label_15;
    QSpacerItem *verticalSpacer;
    QPushButton *clear_r_Button;
    QCheckBox *show16Box;
    QPushButton *send_Button;
    QCheckBox *send16Box;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_3 = new QVBoxLayout(centralwidget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tebWidget = new QTabWidget(centralwidget);
        tebWidget->setObjectName(QString::fromUtf8("tebWidget"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        tebWidget->setFont(font);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_6 = new QVBoxLayout(tab);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        qwtPlot = new QwtPlot(tab);
        qwtPlot->setObjectName(QString::fromUtf8("qwtPlot"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(qwtPlot->sizePolicy().hasHeightForWidth());
        qwtPlot->setSizePolicy(sizePolicy);

        verticalLayout_6->addWidget(qwtPlot);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Segoe UI Symbol"));
        label_2->setFont(font1);

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        pEdit = new QLineEdit(tab);
        pEdit->setObjectName(QString::fromUtf8("pEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pEdit->sizePolicy().hasHeightForWidth());
        pEdit->setSizePolicy(sizePolicy1);
        pEdit->setFont(font1);

        formLayout->setWidget(0, QFormLayout::FieldRole, pEdit);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font1);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_3);

        iEdit = new QLineEdit(tab);
        iEdit->setObjectName(QString::fromUtf8("iEdit"));
        sizePolicy1.setHeightForWidth(iEdit->sizePolicy().hasHeightForWidth());
        iEdit->setSizePolicy(sizePolicy1);
        iEdit->setFont(font1);

        formLayout->setWidget(1, QFormLayout::FieldRole, iEdit);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font1);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_4);

        dEdit = new QLineEdit(tab);
        dEdit->setObjectName(QString::fromUtf8("dEdit"));
        sizePolicy1.setHeightForWidth(dEdit->sizePolicy().hasHeightForWidth());
        dEdit->setSizePolicy(sizePolicy1);
        dEdit->setFont(font1);

        formLayout->setWidget(2, QFormLayout::FieldRole, dEdit);

        sendPIDButton = new QPushButton(tab);
        sendPIDButton->setObjectName(QString::fromUtf8("sendPIDButton"));

        formLayout->setWidget(3, QFormLayout::FieldRole, sendPIDButton);


        horizontalLayout->addLayout(formLayout);

        horizontalSpacer = new QSpacerItem(40, 18, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_8 = new QLabel(tab);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_2->addWidget(label_8, 0, 0, 1, 1);

        ymaxSpinBox = new QDoubleSpinBox(tab);
        ymaxSpinBox->setObjectName(QString::fromUtf8("ymaxSpinBox"));
        sizePolicy1.setHeightForWidth(ymaxSpinBox->sizePolicy().hasHeightForWidth());
        ymaxSpinBox->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(ymaxSpinBox, 0, 1, 1, 1);

        label_16 = new QLabel(tab);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_2->addWidget(label_16, 1, 0, 1, 1);

        yminSpinBox = new QDoubleSpinBox(tab);
        yminSpinBox->setObjectName(QString::fromUtf8("yminSpinBox"));
        sizePolicy1.setHeightForWidth(yminSpinBox->sizePolicy().hasHeightForWidth());
        yminSpinBox->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(yminSpinBox, 1, 1, 1, 1);


        verticalLayout_4->addLayout(gridLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_7 = new QLabel(tab);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(label_7);

        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(label);

        label_17 = new QLabel(tab);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        sizePolicy2.setHeightForWidth(label_17->sizePolicy().hasHeightForWidth());
        label_17->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(label_17);


        verticalLayout_4->addLayout(horizontalLayout_3);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_3->addWidget(label_5, 0, 0, 1, 1);

        peaklabel = new QLabel(tab);
        peaklabel->setObjectName(QString::fromUtf8("peaklabel"));

        gridLayout_3->addWidget(peaklabel, 0, 1, 1, 1);

        label_6 = new QLabel(tab);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_3->addWidget(label_6, 1, 0, 1, 1);

        valleylabel = new QLabel(tab);
        valleylabel->setObjectName(QString::fromUtf8("valleylabel"));

        gridLayout_3->addWidget(valleylabel, 1, 1, 1, 1);


        verticalLayout_4->addLayout(gridLayout_3);


        horizontalLayout->addLayout(verticalLayout_4);

        horizontalSpacer_2 = new QSpacerItem(40, 18, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        startButton = new QPushButton(tab);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        sizePolicy2.setHeightForWidth(startButton->sizePolicy().hasHeightForWidth());
        startButton->setSizePolicy(sizePolicy2);

        verticalLayout->addWidget(startButton);

        pushButton_2 = new QPushButton(tab);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        sizePolicy2.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy2);

        verticalLayout->addWidget(pushButton_2);

        fixButton = new QPushButton(tab);
        fixButton->setObjectName(QString::fromUtf8("fixButton"));
        sizePolicy2.setHeightForWidth(fixButton->sizePolicy().hasHeightForWidth());
        fixButton->setSizePolicy(sizePolicy2);

        verticalLayout->addWidget(fixButton);

        warninglabel = new QLabel(tab);
        warninglabel->setObjectName(QString::fromUtf8("warninglabel"));

        verticalLayout->addWidget(warninglabel);

        resetButton = new QPushButton(tab);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));
        sizePolicy2.setHeightForWidth(resetButton->sizePolicy().hasHeightForWidth());
        resetButton->setSizePolicy(sizePolicy2);

        verticalLayout->addWidget(resetButton);


        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_6->addLayout(horizontalLayout);

        tebWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        horizontalLayout_2 = new QHBoxLayout(tab_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        verticalLayout_5->addWidget(label_9);

        ReceiveEdit = new QTextEdit(tab_2);
        ReceiveEdit->setObjectName(QString::fromUtf8("ReceiveEdit"));

        verticalLayout_5->addWidget(ReceiveEdit);

        label_10 = new QLabel(tab_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        verticalLayout_5->addWidget(label_10);

        SendEdit = new QTextEdit(tab_2);
        SendEdit->setObjectName(QString::fromUtf8("SendEdit"));

        verticalLayout_5->addWidget(SendEdit);


        horizontalLayout_2->addLayout(verticalLayout_5);

        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        bandRateBox = new QComboBox(groupBox);
        bandRateBox->addItem(QString());
        bandRateBox->setObjectName(QString::fromUtf8("bandRateBox"));

        gridLayout->addWidget(bandRateBox, 1, 1, 1, 1);

        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 2, 0, 1, 1);

        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout->addWidget(label_12, 1, 0, 1, 1);

        open_Button = new QPushButton(groupBox);
        open_Button->setObjectName(QString::fromUtf8("open_Button"));

        gridLayout->addWidget(open_Button, 5, 1, 1, 1);

        stopBitBox = new QComboBox(groupBox);
        stopBitBox->addItem(QString());
        stopBitBox->addItem(QString());
        stopBitBox->setObjectName(QString::fromUtf8("stopBitBox"));

        gridLayout->addWidget(stopBitBox, 2, 1, 1, 1);

        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout->addWidget(label_13, 4, 0, 1, 1);

        comBox = new QComboBox(groupBox);
        comBox->setObjectName(QString::fromUtf8("comBox"));

        gridLayout->addWidget(comBox, 0, 1, 1, 1);

        search_Button = new QPushButton(groupBox);
        search_Button->setObjectName(QString::fromUtf8("search_Button"));

        gridLayout->addWidget(search_Button, 0, 0, 1, 1);

        checkoutBox = new QComboBox(groupBox);
        checkoutBox->addItem(QString());
        checkoutBox->addItem(QString());
        checkoutBox->addItem(QString());
        checkoutBox->setObjectName(QString::fromUtf8("checkoutBox"));

        gridLayout->addWidget(checkoutBox, 4, 1, 1, 1);

        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout->addWidget(label_14, 3, 0, 1, 1);

        dataBitBox = new QComboBox(groupBox);
        dataBitBox->addItem(QString());
        dataBitBox->addItem(QString());
        dataBitBox->addItem(QString());
        dataBitBox->addItem(QString());
        dataBitBox->setObjectName(QString::fromUtf8("dataBitBox"));

        gridLayout->addWidget(dataBitBox, 3, 1, 1, 1);

        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout->addWidget(label_15, 5, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(20, 127, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        clear_r_Button = new QPushButton(groupBox);
        clear_r_Button->setObjectName(QString::fromUtf8("clear_r_Button"));

        verticalLayout_2->addWidget(clear_r_Button);

        show16Box = new QCheckBox(groupBox);
        show16Box->setObjectName(QString::fromUtf8("show16Box"));

        verticalLayout_2->addWidget(show16Box);

        send_Button = new QPushButton(groupBox);
        send_Button->setObjectName(QString::fromUtf8("send_Button"));

        verticalLayout_2->addWidget(send_Button);

        send16Box = new QCheckBox(groupBox);
        send16Box->setObjectName(QString::fromUtf8("send16Box"));

        verticalLayout_2->addWidget(send16Box);


        horizontalLayout_2->addWidget(groupBox);

        tebWidget->addTab(tab_2, QString());

        verticalLayout_3->addWidget(tebWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tebWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "P:", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "I:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "D:", nullptr));
        sendPIDButton->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Ymax:", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Ymin:", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\346\227\266\351\227\264\357\274\232", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "s", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\345\263\260\345\200\274\347\202\271\357\274\232", nullptr));
        peaklabel->setText(QCoreApplication::translate("MainWindow", "( 0 , 0 )", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\350\260\267\345\200\274\347\202\271\357\274\232", nullptr));
        valleylabel->setText(QCoreApplication::translate("MainWindow", "( 0 , 0 )", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "\345\244\215\344\275\215", nullptr));
        fixButton->setText(QCoreApplication::translate("MainWindow", "\345\233\272\345\256\232", nullptr));
        warninglabel->setText(QCoreApplication::translate("MainWindow", "\346\234\200\345\260\217\350\267\250\345\272\246\344\270\2720.5\344\270\252\345\215\225\344\275\215", nullptr));
        resetButton->setText(QCoreApplication::translate("MainWindow", "\351\207\215\347\275\256", nullptr));
        tebWidget->setTabText(tebWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "PID\350\260\203\350\257\225", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\346\216\245\346\224\266\347\252\227\345\217\243", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201\347\252\227\345\217\243", nullptr));
        groupBox->setTitle(QString());
        bandRateBox->setItemText(0, QCoreApplication::translate("MainWindow", "115200", nullptr));

        label_11->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\344\275\215", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207", nullptr));
        open_Button->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        stopBitBox->setItemText(0, QCoreApplication::translate("MainWindow", "1", nullptr));
        stopBitBox->setItemText(1, QCoreApplication::translate("MainWindow", "2", nullptr));

        label_13->setText(QCoreApplication::translate("MainWindow", "\345\245\207\345\201\266\346\240\241\351\252\214", nullptr));
        search_Button->setText(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\346\243\200\346\265\213", nullptr));
        checkoutBox->setItemText(0, QCoreApplication::translate("MainWindow", "\346\227\240", nullptr));
        checkoutBox->setItemText(1, QCoreApplication::translate("MainWindow", "\345\245\207\346\240\241\351\252\214", nullptr));
        checkoutBox->setItemText(2, QCoreApplication::translate("MainWindow", "\345\201\266\346\240\241\351\252\214", nullptr));

        label_14->setText(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\344\275\215", nullptr));
        dataBitBox->setItemText(0, QCoreApplication::translate("MainWindow", "8", nullptr));
        dataBitBox->setItemText(1, QCoreApplication::translate("MainWindow", "7", nullptr));
        dataBitBox->setItemText(2, QCoreApplication::translate("MainWindow", "6", nullptr));
        dataBitBox->setItemText(3, QCoreApplication::translate("MainWindow", "5", nullptr));

        label_15->setText(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\346\223\215\344\275\234", nullptr));
        clear_r_Button->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\346\216\245\346\224\266", nullptr));
        show16Box->setText(QCoreApplication::translate("MainWindow", "\345\215\201\345\205\255\350\277\233\345\210\266\346\230\276\347\244\272", nullptr));
        send_Button->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        send16Box->setText(QCoreApplication::translate("MainWindow", "\345\215\201\345\205\255\350\277\233\345\210\266\345\217\221\351\200\201", nullptr));
        tebWidget->setTabText(tebWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\351\200\232\350\256\257", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
