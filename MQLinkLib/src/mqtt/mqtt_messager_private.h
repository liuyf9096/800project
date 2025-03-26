#ifndef MQTT_MESSAGER_PRIVATE_H
#define MQTT_MESSAGER_PRIVATE_H

#include <QObject>

class MqttMessagerPrivate : public QObject
{
    Q_OBJECT
public:
    explicit MqttMessagerPrivate(QObject *parent = nullptr);

signals:
};

#endif // MQTT_MESSAGER_PRIVATE_H
