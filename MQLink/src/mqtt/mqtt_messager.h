#ifndef MQTT_MESSAGER_H
#define MQTT_MESSAGER_H

#include <QObject>
#include "mqtt_client.h"
#include "mqtt_server.h"

class MqttMessager : public QObject
{
    Q_OBJECT
public:
    static MqttMessager *GetInstance();

    MqttClient *client;
    MqttServer *server;

signals:
private:
    explicit MqttMessager(QObject *parent = nullptr);
    Q_DISABLE_COPY(MqttMessager)
    ~MqttMessager();
};

#endif // MQTT_MESSAGER_H
