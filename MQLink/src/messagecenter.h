#ifndef MESSAGECENTER_H
#define MESSAGECENTER_H

#include <QObject>
#include "mqtt/mqtt_messager.h"
#include "zmq/zmq_messager.h"

class MessageCenter : public QObject
{
    Q_OBJECT
public:
    static MessageCenter *GetInstance();

    void networkAutoSetup();

    MqttClient *mqttClient;
    MqttServer *mqttServer;

    ZmqClient *zmqClient;
    ZmqServer *zmqServer;

private:
    explicit MessageCenter(QObject *parent = nullptr);
    Q_DISABLE_COPY(MessageCenter)
    ~MessageCenter();

    QString m_id;

    QTimer *zmqTimer;
    QTimer *mqttTimer;

    bool m_zmqAutoTest{false};
    bool m_mqttAutoTest{false};
    QString zmqATContent;
    QString mqttATTopic;
    QString mqttATContent;

    void _timer_init();

private slots:
    void onZmqTimeout_slot();
    void onMqttTimeout_slot();
};

#endif // MESSAGECENTER_H
