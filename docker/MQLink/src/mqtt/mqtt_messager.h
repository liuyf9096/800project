#ifndef MQTT_MESSAGER_H
#define MQTT_MESSAGER_H

#include "mqtt_client.h"
#include "mqtt_server.h"

#include <QObject>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>

class MqttMessager : public QObject
{
    Q_OBJECT
public:
    static MqttMessager *GetInstance();

    MqttClient *client;
    MqttServer *server;

    void setAutoTest(QJsonObject obj);

private:
    explicit MqttMessager(QObject *parent = nullptr);
    Q_DISABLE_COPY(MqttMessager)
    ~MqttMessager();

    QString m_id;

    bool m_autoTest{false};
    QTimer *autoTestTimer;

    QString mqttATTopic;
    QString mqttMessageType;
    QJsonArray mqttATMessageArr;
    QJsonArray mqttATFilePathArr;
    int m_counter{0};

private slots:
    void onAutoTestTimeout_slot();
};

#endif // MQTT_MESSAGER_H
