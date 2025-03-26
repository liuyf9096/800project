#ifndef MESSAGECENTER_H
#define MESSAGECENTER_H

#include <QObject>
// #include "mosquitto.h"

class QSocketNotifier;
class MessageCenter : public QObject
{
    Q_OBJECT
public:
    static MessageCenter *GetInstance();

    void connectServer(qint16 port = 1883, QString ip = "localhost", quint16 timeout = 60);
    void subscribeTopic(QString topic);
    void unsubscribeTopic(QString topic);

    // void handleMessage(const struct mosquitto_message *msg);
    void handleConnect(int rc);

private:
    explicit MessageCenter(QObject *parent = nullptr);
    Q_DISABLE_COPY(MessageCenter)
    ~MessageCenter();

    QSocketNotifier *m_notifier = nullptr;
    bool m_isConnected{false};

signals:

};

#endif // MESSAGECENTER_H
