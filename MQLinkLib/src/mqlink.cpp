#include "mqlink.h"
#include "mqlink_private.h"
#include "f_common.h"
#include "settings/f_settings.h"
#include "logger/f_log_server.h"

#include "zmq_messager_private.h"
#include "mqtt_messager_private.h"

#include <QDebug>

MQLink::MQLink() : d_ptr(std::make_unique<MQLinkPrivate>())
{

}

MQLink::~MQLink()
{
    qDebug() << __FUNCTION__;
}

std::string MQLink::getDeviceId()
{
    QString str = FSettings::GetInstance()->deviceId();
    return str.toStdString();
}

std::string MQLink::getGroupId()
{
    QString str = FSettings::GetInstance()->groupId();
    return str.toStdString();
}

MQLinkPrivate::MQLinkPrivate(QObject *parent)
    : QObject{parent}
{
    FCommon::GetInstance()->printSystemInfo();

    FLogServer::GetInstance();
    FSettings::GetInstance();

    logInit();
    downloadPathInit();
}

MQLinkPrivate::~MQLinkPrivate()
{
    qDebug() << __FUNCTION__;
}

void MQLinkPrivate::logInit()
{
    auto settings = FSettings::GetInstance();
    auto logger = FLogServer::GetInstance();
    logger->setMaxLogFileCount(settings->MaxLogFileCount());
    if (settings->logContentType() == 0) {
        logger->setMessageFlags(FLogServer::Content);
    } else {
        logger->setMessageFlags(FLogServer::TimeTypeContent);
    }

    if (settings->isLogServerEnable()) {
        logger->saveToTxtEnable(true, settings->logindex());
        logger->start();
    }
}

void MQLinkPrivate::downloadPathInit()
{
    QDir projectDir(QDir::currentPath());
    QString downloadFolder = "download";

    if (!projectDir.exists(downloadFolder)) {
        if (projectDir.mkdir(downloadFolder)) {
            qDebug() << "Download folder created successfully!";
        } else {
            qDebug() << "Failed to create download folder!";
        }
    }
}

/* ZMQ ZMQ ZMQ */
ZmqMessagerPrivate::ZmqMessagerPrivate(QObject *parent)
    : QObject{parent}
{
    client = new ZmqClient(this);
    server = new ZmqServer(this);
    connect(server, &ZmqServer::onReceiveMessage_signal,
            this, &ZmqMessagerPrivate::handleReceiveMessage_slot);
}

void ZmqMessagerPrivate::handleReceiveMessage_slot(QString message)
{
    if (m_callback) {
        m_callback(message.toStdString());
    }
}

ZmqMessager::ZmqMessager() : d_ptr(std::make_unique<ZmqMessagerPrivate>())
{
    int port = FSettings::GetInstance()->getZmqServerPort();
    if (port == 0) {
        port = 5555;
    }
    QString address = QString("tcp://*:%1").arg(port);
    d_ptr->server->bindAddress(address);

    QDir dir(QDir::currentPath());
    dir.cd("download");
    d_ptr->server->setDownloadPath(dir.absolutePath());
}

ZmqMessager::~ZmqMessager()
{

}

void ZmqMessager::connect(const std::string &address, int port)
{
    QString str = QString("tcp://%1:%2").arg(QString::fromStdString(address)).arg(port);
    d_ptr->client->connectServer(str);
}

void ZmqMessager::disconnect()
{
    d_ptr->client->disconnect();
}

void ZmqMessager::sendMessage(const std::string &message)
{
    d_ptr->client->sendMessage(QString::fromStdString(message));
}

bool ZmqMessager::sendFileContent(const std::string &fileName, bool isBinary)
{
    if (isBinary) {
        return d_ptr->client->sendBinaryFile(QString::fromStdString(fileName));
    } else {
        return d_ptr->client->sendFileContent(QString::fromStdString(fileName));
    }

    return false;
}

void ZmqMessager::setReceiveFilePath(const std::string &filePath)
{
    d_ptr->server->setDownloadPath(QString::fromStdString(filePath));
}

void ZmqMessager::setCallback(ZmqMessageCallback callback)
{
    d_ptr->m_callback = std::move(callback);
}

// /* MQTT MQTT MQTT */
MqttMessagerPrivate::MqttMessagerPrivate(QObject *parent)
    : QObject{parent}
{
    client = new MqttClient(this);
    server = new MqttServer(this);
    QObject::connect(client, &MqttClient::onReceiveMessage_signal,
            this, &MqttMessagerPrivate::handleReceiveMessage_slot);
    QObject::connect(client, &MqttClient::onReceiveRawData_signal,
                     this, &MqttMessagerPrivate::handleReceiveFile_slot);
}

void MqttMessagerPrivate::handleReceiveMessage_slot(QString topic, QString message)
{
    if (m_msgCallback) {
        m_msgCallback(topic.toStdString(), message.toStdString());
    }
}

void MqttMessagerPrivate::handleReceiveFile_slot(QString topic, QByteArray data)
{
    if (m_fileCallback) {
        std::string stdData(data.constData(), data.size());
        m_fileCallback(topic.toStdString(), stdData);
    }
}

MqttMessager::MqttMessager(): d_ptr(std::make_unique<MqttMessagerPrivate>())
{

}

MqttMessager::~MqttMessager()
{

}

void MqttMessager::connect(const std::string &address, int port)
{
    d_ptr->client->connect(address, port);
}

void MqttMessager::disconnect()
{
    d_ptr->client->disconnect();
}

void MqttMessager::publish(const std::string &topic, const std::string &message)
{
    d_ptr->client->publish(topic, message);
}

bool MqttMessager::sendFileContent(const std::string& topic, const std::string& fileName)
{
    return d_ptr->client->sendFileContent(topic, fileName);
}

void MqttMessager::setReceiveFilePath(const std::string &filePath)
{
    // d_ptr->server->setDownloadPath(QString::fromStdString(filePath));
}

void MqttMessager::setMsgCallback(MqttMessageCallback callback)
{
    d_ptr->m_msgCallback = std::move(callback);
}

void MqttMessager::setFileCallback(MqttFileCallback callback)
{
    d_ptr->m_fileCallback = std::move(callback);
}
