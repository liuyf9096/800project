#ifndef ZMQ_MESSAGER_H
#define ZMQ_MESSAGER_H

#include <QObject>
#include "zmq_client.h"
#include "zmq_server.h"
#include <QTimer>
#include <QJsonObject>

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
    QString zmqATContent;
    int zmqFileNum{0};
    QString testType;
    QString zmqFileName_1;
    QString zmqFileName_2;
    QString zmqFileName_3;

private slots:
    void onAutoTestTimeout_slot();
};

#endif // ZMQ_MESSAGER_H
