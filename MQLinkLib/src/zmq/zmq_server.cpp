#include "zmq_server.h"
#include <zmq.h>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

ZmqServer::ZmqServer(QObject *parent)
    : QObject{parent}
    , m_context(1)
    , m_socket(m_context, zmq::socket_type::router)
    , m_mode(ZMQ_S_ROUTER)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(50);
    connect(m_timer, &QTimer::timeout, this, &ZmqServer::onTimeout_slot);
}

ZmqServer::~ZmqServer()
{
    m_socket.close();
    qDebug() << __FUNCTION__;
}

void ZmqServer::setClientMode(ZmqServerMode mode)
{
    m_socket.close();

    m_mode = mode;

    if (m_mode == ZMQ_S_REP) {
        m_socket = zmq::socket_t(m_context, zmq::socket_type::rep);
        qDebug() << "Server Set ZMQ_REP mode.";
    } else if (m_mode == ZMQ_S_PUB) {
        m_socket = zmq::socket_t(m_context, zmq::socket_type::pub);
        qDebug() << "Server Set ZMQ_PUB mode.";
    } else if (m_mode == ZMQ_S_PUSH) {
        m_socket = zmq::socket_t(m_context, zmq::socket_type::push);
        qDebug() << "Server Set ZMQ_PUSH mode.";
    } else if (m_mode == ZMQ_S_ROUTER) {
        m_socket = zmq::socket_t(m_context, zmq::socket_type::router);
        qDebug() << "Server Set ZMQ_ROUTER mode.";
    } else {
        m_mode = ZMQ_S_Undefine;
        qDebug() << "Error";
    }
}

void ZmqServer::setLoopTimer(int msec)
{
    m_timer->setInterval(msec);
}

void ZmqServer::bindAddress(QString address)
{
    m_socket.bind(address.toLocal8Bit().constData());
    m_timer->start(20);
    qDebug() << "Server is listening on" << address;
}

void ZmqServer::sendMessage(QString clientId, QString message)
{
    if (m_clientIdentity.size() == 0) {
        return;
    }

    m_socket.send(m_clientIdentity, zmq::send_flags::sndmore);

    zmq::message_t reply(message.toUtf8().constData(), message.toUtf8().size());
    m_socket.send(reply, zmq::send_flags::none);
    qDebug() << ">>[Zmq Server]" << message;
}

void ZmqServer::handleReqRepLoop()
{
    zmq::message_t request;
    zmq::recv_flags flags = zmq::recv_flags::dontwait;

    zmq::recv_result_t result = m_socket.recv(m_clientIdentity, flags);
    if (result) {
        result = m_socket.recv(request, flags);
        if (result) {
            QString message = request.to_string().c_str();
            qDebug() << ">>[Zmq Server]" << message;
            emit onReceiveMessage_signal(message);

            // send respond
            zmq::message_t reply(5);
            memcpy(reply.data(), "World", 5);
            m_socket.send(reply, zmq::send_flags::none);
        }
    }
}

void ZmqServer::handlePubSubLoop()
{

}

void ZmqServer::handlePushPullLoop()
{

}

void ZmqServer::handleRouterDealerLoop()
{
    zmq::message_t request;
    zmq::recv_flags flags = zmq::recv_flags::dontwait;

    zmq::recv_result_t result = m_socket.recv(m_clientIdentity, flags);
    if (result) {
        result = m_socket.recv(request, flags);
        if (result) {
            QByteArray headerData(static_cast<const char*>(request.data()), request.size());
            QDataStream stream(&headerData, QIODevice::ReadOnly);
            stream.setByteOrder(QDataStream::BigEndian);

            quint32 magicNumber;
            stream >> magicNumber;

            /* receive file content */
            if (magicNumber == 0xB1B2F1F2)
            {
                quint16 fileNameLength;
                QString fileName;
                quint32 fileSize;

                stream >> fileNameLength;
                QByteArray fileNameData(fileNameLength, 0);
                stream.readRawData(fileNameData.data(), fileNameLength);
                fileName = QString::fromUtf8(fileNameData);

                stream >> fileSize;
                qDebug() << "Received file header:" << fileName << fileSize;

                zmq::message_t fileDataMsg(fileSize);
                result = m_socket.recv(fileDataMsg, flags);
                if (result) {
                    QByteArray fileData(static_cast<const char*>(fileDataMsg.data()), fileDataMsg.size());

                    /* save file content */
                    QFile outFile(fileName);
                    if (outFile.open(QIODevice::WriteOnly)) {
                        outFile.write(fileData);
                        outFile.close();
                        qDebug() << "File received and saved:" << fileName;
                    } else {
                        qDebug() << "Failed to save file:" << fileName;
                    }
                }
            }
            /* receive text */
            else if (magicNumber == 0xB1B2F1F3)
            {
                quint16 messageSize;
                stream >> messageSize;
                QByteArray messageData(messageSize, 0);
                stream.readRawData(messageData.data(), messageSize);

                QString message = QString::fromUtf8(messageData);
                qDebug() << ">>[Zmq Server]" << message;
                emit onReceiveMessage_signal(message);
            } else {
                qDebug() << "Unknown message type!";
            }
        }
    }
}

void ZmqServer::onTimeout_slot()
{
    if (m_socket == nullptr) {
        return;
    }

    if (m_mode == ZMQ_S_REP) {
        handleReqRepLoop();
    } else if (m_mode == ZMQ_S_PUB) {
        handlePubSubLoop();
    } else if (m_mode == ZMQ_S_PUSH) {
        handlePushPullLoop();
    } else if (m_mode == ZMQ_S_ROUTER) {
        handleRouterDealerLoop();
    }
}
