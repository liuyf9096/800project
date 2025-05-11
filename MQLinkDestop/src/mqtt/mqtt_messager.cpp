#include "mqtt_messager.h"

MqttMessager *MqttMessager::GetInstance()
{
#if 0
    static MqttMessager instance;
    return &instance;
#else
    static MqttMessager *instance = nullptr;
    if (instance == nullptr) {
        instance = new MqttMessager();
    }
    return instance;
#endif
}

MqttMessager::MqttMessager(QObject *parent)
    : QObject{parent}
{
    client = new MqttClient(this);
    server = new MqttServer(this);
    connect(client, &MqttClient::onReceiveMessage_signal, this, &MqttMessager::onReceiveMessage_signal);

    autoTestTimer = new QTimer(this);
    autoTestTimer->setInterval(1000);
    connect(autoTestTimer, &QTimer::timeout, this, &MqttMessager::onAutoTestTimeout_slot);
}

MqttMessager::~MqttMessager()
{
    qDebug() << __FUNCTION__;
}

void MqttMessager::setAutoTest(QJsonObject obj)
{
    QJsonObject clientObj = obj.value("client").toObject();
    QJsonObject serverObj = obj.value("server").toObject();

    qDebug() << __FUNCTION__ << clientObj << serverObj;

    /* Mqtt Client*/
    bool clientEn = clientObj.value("enable").toBool();
    if (clientEn) {
        QString ip = clientObj.value("ip").toString();
        int port = clientObj.value("port").toInt();

        client->connect(ip, port);
        bool autoTest = clientObj.value("autoTest").toBool();
        if (autoTest) {
            int interval = clientObj.value("autoTest_interval").toInt();
            m_autoTest = true;
            autoTestTimer->start(interval);

            mqttATTopic = clientObj.value("autoTest_topic").toString();
            mqttATContent = clientObj.value("autoTest_content").toString();
            qDebug() << "Mqtt Start Auto Test.";
        }
    }

    /* Mqtt Server*/
    bool serverEn = serverObj.value("enable").toBool();
    if (serverEn == true) {
        // int port = serverObj.value("port").toInt();
    }
}

void MqttMessager::onAutoTestTimeout_slot()
{
    static int n = 1;
    QString current_time = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    client->publish(mqttATTopic, QString("[%1](%2) %3 : %4").arg(current_time, m_id).arg(n++).arg(mqttATContent));
}

