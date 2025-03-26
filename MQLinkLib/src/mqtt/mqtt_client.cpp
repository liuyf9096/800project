#include "mqtt_client.h"
#include <QFile>
#include <QFileInfo>
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

bool MqttClient::sendFileContent(QString topic, QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << file.errorString();
        return false;
    }

    QByteArray fileData = file.readAll();
    file.close();

    int rc = mosquitto_publish(mosq, nullptr, topic.toUtf8().constData(),
                               fileData.size(), fileData.constData(), 1, false);

    if (rc != MOSQ_ERR_SUCCESS) {
        qWarning() << "Publish failed:" << mosquitto_strerror(rc);
        return false;
    } else {
        qDebug() << "File sent successfully! Size:" << fileData.size() << "bytes";
        return true;
    }
}

bool MqttClient::sendFileContent(const std::string topic, const std::string filePath)
{
    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << file.errorString();
        return false;
    }

    QByteArray fileData = file.readAll();
    file.close();

    int rc = mosquitto_publish(mosq, nullptr, topic.c_str(),
                               fileData.size(), fileData.constData(), 1, false);

    if (rc != MOSQ_ERR_SUCCESS) {
        qWarning() << "Publish failed:" << mosquitto_strerror(rc);
        return false;
    } else {
        qDebug() << "File sent successfully! Size:" << fileData.size() << "bytes";
        return true;
    }
}

void MqttClient::onTimeout_slot()
{
    if (mosq) {
        mosquitto_loop(mosq, -1, 1);
    }
}

/* Static Method */
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

    QByteArray data(static_cast<const char*>(msg->payload), msg->payloadlen);
    QString message = QString::fromUtf8(data);
    QString topic = msg->topic;

    qDebug() << QString(">>[Topic:%1]%2").arg(topic, message);

    // qDebug() << "Received message on topic:" << msg->topic << ":"
    //          << QString::fromUtf8((char*)msg->payload, msg->payloadlen);

    auto instance = static_cast<MqttClient*>(obj);
    QMetaObject::invokeMethod(
        instance,
        "onReceiveMessage_signal",
        Qt::QueuedConnection,
        Q_ARG(QString, topic),
        Q_ARG(QString, message));

    QMetaObject::invokeMethod(
        instance,
        "onReceiveRawData_signal",
        Qt::QueuedConnection,
        Q_ARG(QString, topic),
        Q_ARG(QByteArray, data));
}

