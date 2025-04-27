#ifndef ZMQ_MESSAGER_H
#define ZMQ_MESSAGER_H

#include <QObject>
#include "zmq_client.h"
#include "zmq_server.h"

class ZmqMessager : public QObject
{
    Q_OBJECT
public:
    static ZmqMessager *GetInstance();

    ZmqClient *client;
    ZmqServer *server;

signals:

private:
    explicit ZmqMessager(QObject *parent = nullptr);
    Q_DISABLE_COPY(ZmqMessager)
    ~ZmqMessager();
};

#endif // ZMQ_MESSAGER_H
