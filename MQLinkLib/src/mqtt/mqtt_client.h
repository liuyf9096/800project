#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <QObject>
#include <QTimer>
#include "mosquitto.h"

class MqttClient : public QObject
{
    Q_OBJECT

    struct mosquitto *mosq = nullptr;
public:
    explicit MqttClient(QObject *parent = nullptr);
    ~MqttClient();

    bool connect(QString address, int port = 1883);
    bool connect(const std::string address, int port = 1883);
    bool disconnect();

    void subscribe(QString topic);
    void subscribe(const std::string topic);

    void publish(QString topic, QString message);
    void publish(const std::string topic, const std::string message);

    bool sendFileContent(QString topic, QString filePath);
    bool sendFileContent(const std::string topic, const std::string filePath);

signals:
    void onReceiveMessage_signal(QString topic, QString message);
    void onReceiveRawData_signal(QString topic, QByteArray data);

private:
    QTimer *m_timer;

private slots:
    void onTimeout_slot();
};

#endif // MQTT_CLIENT_H
