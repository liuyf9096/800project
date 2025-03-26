#include "messagecenter.h"
#include <QSocketNotifier>
#include <QThread>
#include <QDateTime>
#include <QJsonObject>
#include <QDebug>

// static mosquitto *mosq = nullptr;

MessageCenter *MessageCenter::GetInstance()
{
    static MessageCenter instance;
    return &instance;
}

static void onConnect(struct mosquitto *mosq, void *obj, int rc);
static void onMessage(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);

MessageCenter::MessageCenter(QObject *parent)
    : QObject{parent}
{
    qDebug() << __FUNCTION__ << QThread::currentThread();
    // mosquitto_lib_init();
    // mosq = mosquitto_new("mqtt-client", true, this);
    // if (!mosq) {
    //     qCritical() << "Failed to create Mosquitto client";
    //     return;
    // }

    // mosquitto_connect_callback_set(mosq, onConnect);
    // mosquitto_message_callback_set(mosq, onMessage);
}

MessageCenter::~MessageCenter()
{
    qDebug() << __FUNCTION__ << QThread::currentThread();

    // if (mosq) {
    //     mosquitto_destroy(mosq);
    //     mosq = nullptr;
    // }

    // if (m_notifier) {
    //     delete m_notifier;
    //     m_notifier = nullptr;
    // }
}

void MessageCenter::connectServer(qint16 port, QString ip, quint16 timeout)
{
    m_isConnected = false;

    // if (mosquitto_connect(mosq, ip.toLocal8Bit(), port, timeout) != MOSQ_ERR_SUCCESS) {
    //     qCritical() << "Failed to connect to MQTT broker";
    //     return;
    // }

    // int fd = mosquitto_socket(mosq);
    // if (fd < 0) {
    //     qCritical() << "Failed to get Mosquitto socket";
    //     return;
    // }

    // m_notifier = new QSocketNotifier(fd, QSocketNotifier::Read, this);
    // connect(m_notifier, &QSocketNotifier::activated, this, []() {
    //     mosquitto_loop(mosq, -1, 1);
    // });
    // connect(m_notifier, &QSocketNotifier::destroyed, this, []() {
    //     qDebug() << "destroy notifier";
    // });

    qDebug().noquote() << QString("Connecting Server (%1:%2)..").arg(ip).arg(port);
}

void MessageCenter::subscribeTopic(QString topic)
{
    // mosquitto_subscribe(mosq, nullptr, topic.toLocal8Bit(), 0);
}

void MessageCenter::unsubscribeTopic(QString topic)
{
    // mosquitto_unsubscribe(mosq, nullptr, topic.toLocal8Bit());
}

// void MessageCenter::handleMessage(const mosquitto_message *msg)
// {
    // QDateTime currentTime = QDateTime::currentDateTime();
    // QString timestamp = currentTime.toString("yyyyMMdd_hh:mm:ss.zzz");

    // QString message = QString::fromUtf8((char*)msg->payload, msg->payloadlen);
    // QString topic = msg->topic;

    // qDebug().noquote() << QString("%1[%2]>>[%3]:%4").arg(timestamp, "ID", topic, message);
// }

void onConnect(mosquitto *mosq, void *obj, int rc)
{
    Q_UNUSED(mosq);
    Q_UNUSED(obj);

    MessageCenter::GetInstance()->handleConnect(rc);
}

void MessageCenter::handleConnect(int rc)
{
    switch (rc) {
    case 0:
        m_isConnected = true;
        qDebug() << "Connected to MQTT broker";

        // Subscribe to a topic
        // mosquitto_subscribe(mosq, nullptr, "test/topic", 0);
        break;
    case 1:
        qDebug() << "Connection refused: Unsupported protocol version";
        break;
    case 2:
        qDebug() << "Connection refused: Invalid client identifier";
        break;
    case 3:
        qDebug() << "Connection refused: Broker unavailable";
        break;
    case 4:
        qDebug() << "Connection refused: Invalid username or password";
        break;
    case 5:
        qDebug() << "Connection refused: Unauthorized";
        break;
    default:
        qDebug() << "Connection failed with unknown code:" << rc;
        break;
    }
}

void onMessage(mosquitto *mosq, void *obj, const mosquitto_message *msg)
{
    Q_UNUSED(mosq);
    Q_UNUSED(obj);

    // MessageCenter::GetInstance()->handleMessage(msg);
}

