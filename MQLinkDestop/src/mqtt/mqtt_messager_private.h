#ifndef MQTT_MESSAGER_PRIVATE_H
#define MQTT_MESSAGER_PRIVATE_H

#include "mqtt_client.h"
#include "mqtt_server.h"
#include <QObject>

using MqttMsgCallback = std::function<void(const std::string& topic, const std::string& message)>;
using MqttFileCallback = std::function<void(const std::string& topic, const std::string& data)>;

class MqttMessagerPrivate : public QObject
{
    Q_OBJECT
public:
    explicit MqttMessagerPrivate(QObject *parent = nullptr);

    MqttClient *client;
    MqttServer *server;

    MqttMsgCallback m_msgCallback;
    MqttFileCallback m_fileCallback;

public slots:
    void handleReceiveMessage_slot(QString topic, QString message);
    void handleReceiveFile_slot(QString topic, QByteArray data);
};

#endif // MQTT_MESSAGER_PRIVATE_H
