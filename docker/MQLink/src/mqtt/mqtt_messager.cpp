#include "mqtt_messager.h"
#include "settings/f_settings.h"
#include "f_common.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

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
    m_id = FSettings::GetInstance()->deviceId();

    client = new MqttClient(m_id, this);
    server = new MqttServer(this);

    autoTestTimer = new QTimer(this);
    autoTestTimer->setInterval(1000);
    connect(autoTestTimer, &QTimer::timeout, this, &MqttMessager::onAutoTestTimeout_slot);

    QJsonObject obj = FCommon::getConfigFileValue("mqtt").toObject();
    if (!obj.isEmpty()) {
        setAutoTest(obj);
    }
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
        /* Connection */
        QString address = clientObj.value("address").toString();
        int port = clientObj.value("port").toInt();
        client->connect(address, port);

        /* Auto Test */
        QJsonObject autotestObj = clientObj.value("autotest").toObject();
        bool autotest_en = autotestObj.value("enable").toBool();
        if (autotest_en) {
            m_autoTest = true;

            int interval = autotestObj.value("interval").toInt();
            autoTestTimer->start(interval);
            mqttATTopic = autotestObj.value("topic").toString();
            mqttMessageType = autotestObj.value("type").toString();
            mqttATMessageArr = autotestObj.value("message").toArray();
            mqttATFilePathArr = autotestObj.value("filepath").toArray();
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

    if (m_counter >= mqttATMessageArr.count()) {
        m_counter = 0;
    }
    QString content = mqttATMessageArr.at(m_counter).toString();
    m_counter++;

    client->publish(mqttATTopic, QString("[%1](%2) %3 : %4").arg(current_time, m_id).arg(n++).arg(content));
}

