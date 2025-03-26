#ifndef MQTT_SERVER_H
#define MQTT_SERVER_H

#include <QObject>

class MqttServer : public QObject
{
    Q_OBJECT
public:
    explicit MqttServer(QObject *parent = nullptr);

signals:
};

#endif // MQTT_SERVER_H
