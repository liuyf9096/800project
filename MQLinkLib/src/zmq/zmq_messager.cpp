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
    server->bindAddress();
}

ZmqMessager::~ZmqMessager()
{
    qDebug() << __FUNCTION__;
}
