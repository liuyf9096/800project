#ifndef MESSAGECENTER_H
#define MESSAGECENTER_H

#include <QObject>

class MqttClient;
class MqttServer;
class ZmqClient;
class ZmqServer;

class MessageCenter : public QObject
{
    Q_OBJECT
public:
    static MessageCenter *GetInstance();

    MqttClient *mqttClient;
    MqttServer *mqttServer;

    ZmqClient *zmqClient;
    ZmqServer *zmqServer;

private:
    explicit MessageCenter(QObject *parent = nullptr);
    Q_DISABLE_COPY(MessageCenter)
    ~MessageCenter();

    QString m_id;
};

#endif // MESSAGECENTER_H
