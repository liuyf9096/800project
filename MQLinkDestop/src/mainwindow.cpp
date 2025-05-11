#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mqtt/mqtt_messager.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto mqtt = MqttMessager::GetInstance();
    connect(mqtt, &MqttMessager::onReceiveMessage_signal, this, &MainWindow::handleMQTTMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleMQTTMessage(const QString &topic, const QString &message)
{
    if (topic == "device/ip") {
        QString str = message;
        int endIndex = str.indexOf(']');
        if (endIndex > 0) {
            QString deviceId = str.mid(1, endIndex - 1);
            QString ipAddr = str.mid(endIndex + 1);
            qDebug() << "Device ID:" << deviceId;
            qDebug() << "IP Address:" << ipAddr;
            ui->uav_1->setIpAddress(ipAddr);
            ui->uav_1->setDeviceId(deviceId);
        }
    }
}
