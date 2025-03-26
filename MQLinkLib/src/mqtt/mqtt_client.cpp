#include "mqtt_client.h"
#include <QDebug>

static void onConnect(struct mosquitto *mosq, void *obj, int rc);
static void onMessage(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);

MqttClient::MqttClient(QObject *parent)
    : QObject{parent}
{
    m_timer = new QTimer(this);
    m_timer->setInterval(50);
    QObject::connect(m_timer, &QTimer::timeout, this, &MqttClient::onTimeout_slot);

    mosquitto_lib_init();
    mosq = mosquitto_new("qt-mqtt-client", true, this);
    if (mosq) {
        mosquitto_connect_callback_set(mosq, onConnect);
        mosquitto_message_callback_set(mosq, onMessage);
    } else {
        qCritical() << "Failed to create Mosquitto client";
    }
}

MqttClient::~MqttClient()
{
    if (mosq) {
        mosquitto_destroy(mosq);
    }
    mosquitto_lib_cleanup();
}

bool MqttClient::connect(QString address, int port)
{
    if (mosquitto_connect(mosq, "localhost", port, 60) == MOSQ_ERR_SUCCESS) {
        qDebug() << "Connected.";
        m_timer->start();
        return true;
    }
    return false;
}

bool MqttClient::connect(const std::string address, int port)
{
    if (mosquitto_connect(mosq, address.c_str(), port, 60) == MOSQ_ERR_SUCCESS) {
        qDebug() << "Connected.";
        m_timer->start();
        return true;
    }
    return false;
}

bool MqttClient::disconnect()
{
    if (mosquitto_disconnect(mosq) == MOSQ_ERR_SUCCESS) {
        return true;
    }
    return false;
}

void MqttClient::subscribe(QString topic)
{
    mosquitto_subscribe(mosq, nullptr, topic.toUtf8().constData(), 0);
}

void MqttClient::subscribe(const std::string topic)
{
    mosquitto_subscribe(mosq, nullptr, topic.c_str(), 0);
}

void MqttClient::publish(QString topic, QString message)
{
    if (mosq) {
        mosquitto_publish(mosq, nullptr, topic.toUtf8().constData(), message.size(), message.toUtf8().constData(), 0, false);
    }
}

void MqttClient::publish(const std::string topic, const std::string message)
{
    if (mosq) {
        mosquitto_publish(mosq, nullptr, topic.c_str(), int(message.length()), message.c_str(), 0, false);
    }
}

void MqttClient::onTimeout_slot()
{
    if (mosq) {
        mosquitto_loop(mosq, -1, 1);
    }
}

void onConnect(mosquitto *mosq, void *obj, int rc)
{
    Q_UNUSED(mosq);
    Q_UNUSED(obj);
    if (rc == 0) {
        qDebug() << "Connected to MQTT broker";

        mosquitto_subscribe(mosq, nullptr, "test/topic", 0);
    } else {
        qCritical() << "Connection failed with code" << rc;
    }
}

void onMessage(mosquitto *mosq, void *obj, const mosquitto_message *msg)
{
    Q_UNUSED(mosq);
    Q_UNUSED(obj);
    qDebug() << "Received message on topic:" << msg->topic << ":"
             << QString::fromUtf8((char*)msg->payload, msg->payloadlen);
}

