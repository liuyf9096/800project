#ifndef ZMQ_SERVER_H
#define ZMQ_SERVER_H

#include <QObject>
#include <QTimer>
#include <QMap>

/*
    REQ-REP:(Client)ZMQ_REQ, (Server)ZMQ_REP
    PUB-SUB:(Client)ZMQ_SUB, (Server)ZMQ_PUB
    PUSH-PULL:(Client)ZMQ_PULL, (Server)ZMQ_PUSH
    ROUTER-DEALER:(Client)ZMQ_DEALER, (Server)ZMQ_ROUTER
*/

enum ZmqServerMode {
    ZMQ_S_Undefine = 0,
    ZMQ_S_REP,
    ZMQ_S_PUB,
    ZMQ_S_PUSH,
    ZMQ_S_ROUTER
};

class ZmqServer : public QObject
{
    Q_OBJECT
public:
    explicit ZmqServer(QObject *parent = nullptr);
    ~ZmqServer();

    void setClientMode(ZmqServerMode mode);
    void setLoopTimer(int msec);

    void bindAddress(QString address = "tcp://*:5555");
    void sendMessage(QString clientId, QString message);

signals:
    void onReceiveMessage_signal(QString message);

private:
    void* m_context = nullptr;
    void* m_socket = nullptr;

    ZmqServerMode m_mode{ZMQ_S_Undefine};

    QTimer *m_timer;
    QByteArray m_identity;
    QMap<QByteArray, QString> clientMap;

    void handleReqRepLoop();
    void handlePubSubLoop();
    void handlePushPullLoop();
    void handleRouterDealerLoop();

private slots:
    void onTimeout_slot();
};

#endif // ZMQ_SERVER_H
