#include "messagecenter.h"
#include <QThread>
#include <QDateTime>
#include <QJsonObject>
#include <QDebug>

MessageCenter *MessageCenter::GetInstance()
{
    static MessageCenter instance;
    return &instance;
}

MessageCenter::MessageCenter(QObject *parent)
    : QObject{parent}
{
    // qDebug() << __FUNCTION__ << QThread::currentThread();

    MqttMessager::GetInstance();
    ZmqMessager::GetInstance();
}

MessageCenter::~MessageCenter()
{
    qDebug() << __FUNCTION__ << QThread::currentThread();
}


