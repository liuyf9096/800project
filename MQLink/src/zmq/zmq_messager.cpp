#include "zmq_messager.h"
#include <QDebug>

ZmqMessager *ZmqMessager::GetInstance()
{
    static ZmqMessager instance;
    return &instance;
}

ZmqMessager::ZmqMessager(QObject *parent)
    : QObject{parent}
{
    client = new ZmqClient(this);
    server = new ZmqServer(this);
}

ZmqMessager::~ZmqMessager()
{
    qDebug() << __FUNCTION__;
}
