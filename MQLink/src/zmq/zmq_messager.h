#ifndef ZMQ_MESSAGER_H
#define ZMQ_MESSAGER_H

#include "zmq_client.h"
#include "zmq_server.h"

#include <QObject>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>

class ZmqMessager : public QObject
{
    Q_OBJECT
public:
    static ZmqMessager *GetInstance();

    ZmqClient *client;
    ZmqServer *server;

    void setAutoTest(QJsonObject obj);

private:
    explicit ZmqMessager(QObject *parent = nullptr);
    Q_DISABLE_COPY(ZmqMessager)
    ~ZmqMessager();

    QString m_id;

    bool m_autoTest{false};
    QTimer *autoTestTimer;

    /* AutoTest */
    QString zmqMessageType;
    QJsonArray zmqATMessageArr;
    QJsonArray zmqATFilePathArr;
    int m_counter{0};

private slots:
    void onAutoTestTimeout_slot();
};

#endif // ZMQ_MESSAGER_H
