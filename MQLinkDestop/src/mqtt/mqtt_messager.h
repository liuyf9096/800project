#ifndef MQTT_MESSAGER_H
#define MQTT_MESSAGER_H

#include <QObject>
#include "mqtt_client.h"
#include "mqtt_server.h"
#include <QTimer>
#include <QJsonObject>

class MqttMessager : public QObject
{
    Q_OBJECT
public:
    static MqttMessager *GetInstance();

    MqttClient *client;
    MqttServer *server;

    void setDeviceId(const QString &id) { m_id = id; }
    void setAutoTest(QJsonObject obj);

signals:
    void onReceiveMessage_signal(QString topic, QString message);

private:
    explicit MqttMessager(QObject *parent = nullptr);
    Q_DISABLE_COPY(MqttMessager)
    ~MqttMessager();

    QString m_id;

    bool m_autoTest{false};
    QTimer *autoTestTimer;

    QString mqttATTopic;
    QString mqttATContent;

private slots:
    void onAutoTestTimeout_slot();
};

#endif // MQTT_MESSAGER_H
