#ifndef ZMQ_CLIENT_H
#define ZMQ_CLIENT_H

#include <QObject>
#include <QTimer>

/*
    REQ-REP:(Client)ZMQ_REQ, (Server)ZMQ_REP
    PUB-SUB:(Client)ZMQ_SUB, (Server)ZMQ_PUB
    PUSH-PULL:(Client)ZMQ_PULL, (Server)ZMQ_PUSH
    ROUTER-DEALER:(Client)ZMQ_DEALER, (Server)ZMQ_ROUTER
*/

enum ZmqClientMode {
    ZMQ_C_Undefine = 0,
    ZMQ_C_REQ,
    ZMQ_C_SUB,
    ZMQ_C_PULL,
    ZMQ_C_DEALER
};

class ZmqClient : public QObject
{
    Q_OBJECT
public:
    explicit ZmqClient(QObject *parent = nullptr);
    ~ZmqClient();

    void setMode(ZmqClientMode mode);
    void setLoopTimer(int msec);

    void connect(QString address = "tcp://localhost:5555");
    void disconnect();
    void sendMessage(QString message);
    quint64 sendMessageWithID(QString message);

private:
    void* m_context = nullptr;
    void* m_socket = nullptr;

    ZmqClientMode m_mode{ZMQ_C_Undefine};
    quint32 m_index{0};

    QString m_serverAddress;
    QTimer *m_timer;

    void receiveloop();
    void receiveRouterLoop();

private slots:
    void onTimeout_slot();
};

#endif // ZMQ_CLIENT_H
