#ifndef MQTT_MESSAGER_PRIVATE_H
#define MQTT_MESSAGER_PRIVATE_H

#include "mqtt_client.h"
#include "mqtt_server.h"
#include <QObject>

class MqttMessagerPrivate : public QObject
{
    Q_OBJECT
public:
    explicit MqttMessagerPrivate(QObject *parent = nullptr);

    MqttClient *client;
    MqttServer *server;

signals:
};

#endif // MQTT_MESSAGER_PRIVATE_H
