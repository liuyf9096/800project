#include "messagecenter.h"
#include <QThread>
#include <QDateTime>
#include <QJsonObject>
#include <QDebug>
#include "settings/f_settings.h"

MessageCenter *MessageCenter::GetInstance()
{
    static MessageCenter instance;
    return &instance;
}

MessageCenter::MessageCenter(QObject *parent)
    : QObject{parent}
{
    // qDebug() << __FUNCTION__ << QThread::currentThread();

    _timer_init();

    mqttClient = MqttMessager::GetInstance()->client;
    mqttServer = MqttMessager::GetInstance()->server;

    zmqClient = ZmqMessager::GetInstance()->client;
    zmqServer = ZmqMessager::GetInstance()->server;

    if (FSettings::GetInstance()->networkAutoSetup() == true) {
        networkAutoSetup();
    }
}

MessageCenter::~MessageCenter()
{
    qDebug() << __FUNCTION__ << QThread::currentThread();
}

void MessageCenter::networkAutoSetup()
{
    qDebug() << __FUNCTION__;

    QJsonObject mqttObj = FSettings::GetInstance()->getMqttObject();
    QJsonObject zmqObj = FSettings::GetInstance()->getZmqObject();

    QJsonObject mqttClientObj = mqttObj.value("client").toObject();
    QJsonObject mqttServerObj = mqttObj.value("server").toObject();

    QJsonObject zmqServerObj = zmqObj.value("server").toObject();
    QJsonObject zmqClientObj = zmqObj.value("client").toObject();

    qDebug() << mqttClientObj << mqttServerObj << zmqServerObj << zmqClientObj;

    /* Zmq Server*/
    bool zmqServerEn = zmqServerObj.value("enable").toBool();
    if (zmqServerEn == true) {
        int port = zmqServerObj.value("port").toInt();

        zmqServer->bindAddress(QString("tcp://*:%1").arg(port));
    }

    /* Zmq Client*/
    bool zmqClientEn = zmqClientObj.value("enable").toBool();
    if (zmqClientEn) {
        QString ip = zmqClientObj.value("ip").toString();
        int port = zmqClientObj.value("port").toInt();

        zmqClient->connectServer(QString("tcp://%1:%2").arg(ip).arg(port));
        bool autoTest = zmqClientObj.value("autoTest").toBool();
        if (autoTest) {
            int interval = zmqClientObj.value("autoTest_interval").toInt();
            m_zmqAutoTest = true;
            zmqTimer->start(interval);
            zmqATContent = zmqClientObj.value("autoTest_content").toString();
            qDebug() << "Zmq Start Auto Test.";
        }
    }

    /* Mqtt Server*/
    bool mqttServerEn = mqttServerObj.value("enable").toBool();
    if (mqttServerEn == true) {
        // int port = mqttServerObj.value("port").toInt();
        // MqttMessager::GetInstance()->server;
    }

    /* Mqtt Client*/
    bool mqttClientEn = mqttClientObj.value("enable").toBool();
    if (mqttClientEn) {
        QString ip = mqttClientObj.value("ip").toString();
        int port = mqttClientObj.value("port").toInt();

        mqttClient->connect(ip, port);
        bool autoTest = mqttClientObj.value("autoTest").toBool();
        if (autoTest) {
            int interval = mqttClientObj.value("autoTest_interval").toInt();
            m_mqttAutoTest = true;
            mqttTimer->start(interval);
            mqttATTopic = mqttClientObj.value("autoTest_topic").toString();
            mqttATContent = mqttClientObj.value("autoTest_content").toString();
            qDebug() << "Mqtt Start Auto Test.";
        }
    }
}

void MessageCenter::_timer_init()
{
    zmqTimer = new QTimer(this);
    mqttTimer = new QTimer(this);

    connect(zmqTimer, &QTimer::timeout, this, &MessageCenter::onZmqTimeout_slot);
    connect(mqttTimer, &QTimer::timeout, this, &MessageCenter::onMqttTimeout_slot);
}

void MessageCenter::onZmqTimeout_slot()
{
    zmqClient->sendMessage(zmqATContent);
}

void MessageCenter::onMqttTimeout_slot()
{
    mqttClient->publish(mqttATTopic, mqttATContent);
}

