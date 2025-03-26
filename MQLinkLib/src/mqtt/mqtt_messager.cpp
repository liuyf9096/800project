#include "mqtt_messager.h"

MqttMessager *MqttMessager::GetInstance()
{
    static MqttMessager instance;
    return &instance;
}

MqttMessager::MqttMessager(QObject *parent)
    : QObject{parent}
{
    client = new MqttClient(this);
    server = new MqttServer(this);
}


