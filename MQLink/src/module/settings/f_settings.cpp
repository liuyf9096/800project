#include "f_settings.h"
#include "f_common.h"

#include <QDir>
#include <QDebug>

FSettings *FSettings::GetInstance()
{
    static FSettings instance;
    return &instance;
}

FSettings::FSettings(QObject *parent) : QObject(parent)
{
    QDir dir(FCommon::getPath("config"));
    QString inifile = "config.ini";

    m_settings = new QSettings(dir.absoluteFilePath(inifile), QSettings::IniFormat, this);

    if (!dir.exists(inifile)) {
#if 0
        QString str = QString("Error! File %1 is missing.").arg(inifile);
        qFatal("%s", str.toLatin1().data());
#else
        qDebug() << "Create New config.ini File:" << inifile;
        defaultValueInit();
#endif
    }

    qInfo() << "config.ini file path:" << m_settings->fileName();
}

FSettings::~FSettings()
{
    qDebug() << __FUNCTION__;
}

void FSettings::defaultValueInit()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timestamp = currentTime.toString("MMddhhmmss");

    m_settings->setValue("deviceID", "uav_" + timestamp);
    m_settings->setValue("groupID", "stevens.cs");

    m_settings->beginGroup("Logger");
    m_settings->setValue("enable", false);
    m_settings->setValue("index", 7);
    m_settings->setValue("content_type", 0);
    m_settings->setValue("max_logfile_count", 20);
    m_settings->endGroup();

#if 0
    m_settings->beginGroup("MQTT_Client");
    m_settings->setValue("enable", false);
    m_settings->setValue("ip", "127.0.0.1");
    m_settings->setValue("port", 1883);
    m_settings->setValue("autoTest", true);
    m_settings->setValue("autoTest_interval", 1000);
    m_settings->setValue("autoTest_topic", "test/topic");
    m_settings->setValue("autoTest_content", "Hello");
    m_settings->endGroup();

    m_settings->beginGroup("MQTT_Server");
    m_settings->setValue("enable", false);
    m_settings->setValue("ip", "127.0.0.1");
    m_settings->setValue("port", 1883);
    m_settings->endGroup();

    m_settings->beginGroup("ZeroMQ_Client");
    m_settings->setValue("enable", true);
    m_settings->setValue("mode", 4);
    m_settings->setValue("ip", "127.0.0.1");
    m_settings->setValue("port", 5555);
    m_settings->setValue("autoTest", true);
    m_settings->setValue("testType", "text");
    m_settings->setValue("autoTest_interval", 1000);
    m_settings->setValue("autoTest_content", "Hello");
    m_settings->setValue("send_file_number", 0);
    m_settings->setValue("send_filepath_1", "");
    m_settings->setValue("send_filepath_2", "");
    m_settings->setValue("send_filepath_3", "");
    m_settings->endGroup();

    m_settings->beginGroup("ZeroMQ_Server");
    m_settings->setValue("enable", true);
    m_settings->setValue("mode", 4);
    m_settings->setValue("ip", "localhost");
    m_settings->setValue("port", 5555);
    m_settings->endGroup();
#endif
}

void FSettings::setValue(const QString &key, const QVariant &value)
{
    m_settings->setValue(key, value);
}

QVariant FSettings::getValue(const QString &key)
{
    return m_settings->value(key);
}

/* function */
QString FSettings::deviceId()
{
    return m_settings->value("deviceID").toString();
}

QString FSettings::groupId()
{
    return m_settings->value("groupID").toString();
}

QJsonObject FSettings::getMqttObject()
{
    QJsonObject obj;

    QJsonObject clientObj;
    clientObj.insert("enable", m_settings->value("MQTT_Client/enable").toBool());
    clientObj.insert("ip", m_settings->value("MQTT_Client/ip").toString());
    clientObj.insert("port", m_settings->value("MQTT_Client/port").toInt());
    clientObj.insert("mode", m_settings->value("MQTT_Client/mode").toInt());
    clientObj.insert("autoTest", m_settings->value("MQTT_Client/autoTest").toBool());
    clientObj.insert("autoTest_interval", m_settings->value("MQTT_Client/autoTest_interval").toInt());
    clientObj.insert("autoTest_topic", m_settings->value("MQTT_Client/autoTest_topic").toString());
    clientObj.insert("autoTest_content", m_settings->value("MQTT_Client/autoTest_content").toString());
    obj.insert("client", clientObj);

    QJsonObject serverObj;
    serverObj.insert("enable", m_settings->value("MQTT_Server/enable").toBool());
    serverObj.insert("ip", m_settings->value("MQTT_Server/ip").toString());
    serverObj.insert("port", m_settings->value("MQTT_Server/port").toInt());
    serverObj.insert("mode", m_settings->value("MQTT_Server/mode").toInt());
    obj.insert("server", serverObj);

    return obj;
}

QJsonObject FSettings::getZmqObject()
{
    QJsonObject obj;

    QJsonObject clientObj;
    clientObj.insert("enable", m_settings->value("ZeroMQ_Client/enable").toBool());
    clientObj.insert("ip", m_settings->value("ZeroMQ_Client/ip").toString());
    clientObj.insert("port", m_settings->value("ZeroMQ_Client/port").toInt());
    clientObj.insert("mode", m_settings->value("ZeroMQ_Client/mode").toInt());
    clientObj.insert("autoTest", m_settings->value("ZeroMQ_Client/autoTest").toBool());
    clientObj.insert("testType", m_settings->value("ZeroMQ_Client/testType").toString());
    clientObj.insert("autoTest_interval", m_settings->value("ZeroMQ_Client/autoTest_interval").toInt());
    clientObj.insert("autoTest_content", m_settings->value("ZeroMQ_Client/autoTest_content").toString());
    clientObj.insert("send_file_number", m_settings->value("ZeroMQ_Client/send_file_number").toInt());
    clientObj.insert("send_filepath_1", m_settings->value("ZeroMQ_Client/send_filepath_1").toString());
    clientObj.insert("send_filepath_2", m_settings->value("ZeroMQ_Client/send_filepath_2").toString());
    clientObj.insert("send_filepath_3", m_settings->value("ZeroMQ_Client/send_filepath_3").toString());
    obj.insert("client", clientObj);

    QJsonObject serverObj;
    serverObj.insert("enable", m_settings->value("ZeroMQ_Server/enable").toBool());
    serverObj.insert("ip", m_settings->value("ZeroMQ_Server/ip").toString());
    serverObj.insert("port", m_settings->value("ZeroMQ_Server/port").toInt());
    serverObj.insert("mode", m_settings->value("ZeroMQ_Server/mode").toInt());
    obj.insert("server", serverObj);

    return obj;
}

quint32 FSettings::logindex()
{
    quint32 index = m_settings->value("Logger/index").toUInt() + 1;
    m_settings->setValue("Logger/index", index);
    m_settings->sync();
    return index;
}

bool FSettings::isLogServerEnable()
{
    return m_settings->value("Logger/enable").toBool();
}

int FSettings::logContentType()
{
    return m_settings->value("Logger/content_type").toInt();
}

quint32 FSettings::MaxLogFileCount()
{
    return m_settings->value("Logger/max_logfile_count").toUInt();
}


