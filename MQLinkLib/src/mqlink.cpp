#include "mqlink.h"
#include "mqlink_private.h"
#include "f_common.h"
#include "settings/f_settings.h"
#include "logger/f_log_server.h"
#include "file/f_file_manager.h"

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

    /* log */
    logInit();
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
    QByteArray data = FFileManager::readFile(QString::fromStdString(fileName), FFileManager::POS_USER);
    if (!data.isEmpty()) {
        if (isBinary) {
            d_ptr->client->sendData(data);
            qDebug() << "data size:" << data.size();
        } else {
            d_ptr->client->sendMessage(data);
        }
    }
    return false;
}

void ZmqMessager::setCallback(MessageCallback callback)
{
    d_ptr->m_callback = std::move(callback);
}

// /* MQTT MQTT MQTT */
MqttMessager::MqttMessager(): d_ptr(std::make_unique<MqttMessagerPrivate>())
{

}

MqttMessager::~MqttMessager()
{

}

MqttMessagerPrivate::MqttMessagerPrivate(QObject *parent)
    : QObject{parent}
{
    qDebug() << "MqttMessagerPrivate";

}

void MqttMessager::connect(const std::string &address)
{

}

void MqttMessager::disconnect()
{

}

uint64_t MqttMessager::sendMessage(const std::string &message)
{
    return 1;
}

