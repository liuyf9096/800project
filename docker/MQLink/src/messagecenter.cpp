#include "messagecenter.h"
#include <QThread>
#include <QDateTime>
#include <QJsonObject>
#include <QDebug>

#include "settings/f_settings.h"
#include "mqtt/mqtt_messager.h"
#include "zmq/zmq_messager.h"

MessageCenter *MessageCenter::GetInstance()
{
    static MessageCenter instance;
    return &instance;
}

MessageCenter::MessageCenter(QObject *parent)
    : QObject{parent}
{
    // qDebug() << __FUNCTION__ << QThread::currentThread();

    m_id = FSettings::GetInstance()->deviceId();

    mqttClient = MqttMessager::GetInstance()->client;
    mqttServer = MqttMessager::GetInstance()->server;

    zmqClient = ZmqMessager::GetInstance()->client;
    zmqServer = ZmqMessager::GetInstance()->server;
}

MessageCenter::~MessageCenter()
{
    qDebug() << __FUNCTION__ /*<< QThread::currentThread()*/;
}
