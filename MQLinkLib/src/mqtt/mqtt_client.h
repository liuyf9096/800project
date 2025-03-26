#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <QObject>

class MqttClient : public QObject
{
    Q_OBJECT
public:
    explicit MqttClient(QObject *parent = nullptr);

signals:
};

#endif // MQTT_CLIENT_H
