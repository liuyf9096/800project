/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *groupBox;
    QPushButton *btn_client_connect;
    QLineEdit *lineEdit_client;
    QPushButton *btn_client_send;
    QGroupBox *groupBox_2;
    QPushButton *btn_server_bind;
    QLineEdit *lineEdit_server;
    QPushButton *btn_server_send;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(20, 40, 371, 221));
        btn_client_connect = new QPushButton(groupBox);
        btn_client_connect->setObjectName("btn_client_connect");
        btn_client_connect->setGeometry(QRect(10, 20, 141, 51));
        lineEdit_client = new QLineEdit(groupBox);
        lineEdit_client->setObjectName("lineEdit_client");
        lineEdit_client->setGeometry(QRect(10, 90, 141, 41));
        btn_client_send = new QPushButton(groupBox);
        btn_client_send->setObjectName("btn_client_send");
        btn_client_send->setGeometry(QRect(180, 80, 141, 51));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(400, 50, 361, 221));
        btn_server_bind = new QPushButton(groupBox_2);
        btn_server_bind->setObjectName("btn_server_bind");
        btn_server_bind->setGeometry(QRect(30, 20, 131, 61));
        lineEdit_server = new QLineEdit(groupBox_2);
        lineEdit_server->setObjectName("lineEdit_server");
        lineEdit_server->setGeometry(QRect(10, 110, 141, 41));
        btn_server_send = new QPushButton(groupBox_2);
        btn_server_send->setObjectName("btn_server_send");
        btn_server_send->setGeometry(QRect(170, 100, 141, 51));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Client", nullptr));
        btn_client_connect->setText(QCoreApplication::translate("MainWindow", "connect server", nullptr));
        btn_client_send->setText(QCoreApplication::translate("MainWindow", "send", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Server", nullptr));
        btn_server_bind->setText(QCoreApplication::translate("MainWindow", "Bind", nullptr));
        btn_server_send->setText(QCoreApplication::translate("MainWindow", "send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
