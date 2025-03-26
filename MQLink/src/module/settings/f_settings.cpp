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
        qDebug() << "Create New Init File:" << inifile;
        defaultValueInit();
#endif
    }

    qInfo() << "app settings file path:" << m_settings->fileName();
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

    m_settings->beginGroup("MQTT");
    m_settings->setValue("ip", "localhost");
    m_settings->setValue("port", 1883);
    m_settings->endGroup();

    m_settings->beginGroup("ZeroMQ_Client");
    m_settings->setValue("mode", 1);
    m_settings->setValue("ip", "localhost");
    m_settings->setValue("port", 1883);
    m_settings->endGroup();

    m_settings->beginGroup("ZeroMQ_Server");
    m_settings->setValue("mode", 1);
    m_settings->setValue("ip", "localhost");
    m_settings->setValue("port", 1883);
    m_settings->endGroup();

    m_settings->beginGroup("Logger");
    m_settings->setValue("Logger_enable", false);
    m_settings->setValue("index", 0);
    m_settings->setValue("max_logfile_count", 100);
    m_settings->endGroup();
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
bool FSettings::isDebugMode()
{
    return m_settings->value("debug").toBool();
}

quint32 FSettings::logindex()
{
    quint32 index = m_settings->value("Log/index").toUInt() + 1;
    m_settings->setValue("Log/index", index);
    return index;
}

bool FSettings::isLogServerEnable()
{
    return m_settings->value("Logger/Logger_enable").toBool();
}

quint32 FSettings::MaxLogFileCount()
{
    return m_settings->value("Logger/max_logfile_count").toUInt();
}

