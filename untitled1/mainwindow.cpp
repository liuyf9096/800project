#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_client = new QMqttClient(this);
    m_client->setHostname("localhost");
    m_client->setPort(static_cast<quint16>(1883));

    connect(m_client, &QMqttClient::stateChanged, this, &MainWindow::updateLogStateChange);
    connect(m_client, &QMqttClient::disconnected, this, &MainWindow::brokerDisconnected);

    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
        const QString content = QDateTime::currentDateTime().toString()
        + QLatin1String(" Received Topic: ")
            + topic.name()
            + QLatin1String(" Message: ")
            + message
            + QLatin1Char('\n');
        qDebug() << ">>" << content;
    });

    connect(m_client, &QMqttClient::pingResponseReceived, this, [this]() {
        const QString content = QDateTime::currentDateTime().toString()
        + QLatin1String(" PingResponse")
            + QLatin1Char('\n');
        qDebug() << "pingResponseReceived" << content;
    });

    m_client->connectToHost();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateLogStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
    + QLatin1String(": State Change")
        + QString::number(m_client->state())
        + QLatin1Char('\n');
    qDebug() << "updateLogStateChange" << content;
    if (m_client->state() == 2) {
        m_client->subscribe(QString("test/topic"));
    }
}

void MainWindow::brokerDisconnected()
{
    qDebug() << "brokerDisconnected";
}

