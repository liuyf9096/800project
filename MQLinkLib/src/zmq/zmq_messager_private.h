#ifndef ZMQ_MESSAGER_PRIVATE_H
#define ZMQ_MESSAGER_PRIVATE_H

#include "zmq_client.h"
#include "zmq_server.h"
#include <QObject>

using ZmqCallback = std::function<void(const std::string&)>;

class ZmqMessagerPrivate : public QObject
{
    Q_OBJECT
public:
    explicit ZmqMessagerPrivate(QObject *parent = nullptr);

    ZmqClient *client;
    ZmqServer *server;

    ZmqCallback m_callback;

public slots:
    void handleReceiveMessage_slot(QString message);
};

#endif // ZMQ_MESSAGER_PRIVATE_H
